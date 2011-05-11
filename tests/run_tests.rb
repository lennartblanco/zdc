#!/usr/bin/ruby

require "rexml/document"
require 'optparse'
include REXML

BuildDirRoot = "build"

class Runner
  attr_reader :passed
  attr_reader :failed

  def initialize(arch)
    @passed = 0
    @failed = 0
    @arch = arch
  end
end

class LocalRunner < Runner
  def run(build_dir, tests)

    orig_dir = Dir.pwd
    Dir.chdir(build_dir)

    for test in tests
      print "Running #{test}..."
      res = `./check_#{test} 2>&1`

      if $? == 0
        print "[ok]\n"
        @passed += 1
      else
        print "error\n\n", res, "\n"
        @failed += 1
      end
    end

    Dir.chdir(orig_dir)
  end
end

# run test binaries via qemu arm user-mode emulation
class QemuUserRunner < Runner
  def run(build_dir, tests)

    # only arm supported currently
    fail "#{@arch} architecture unsupported by #{self.class.to_s} runner" \
      unless @arch == "arm"

    orig_dir = Dir.pwd
    Dir.chdir(build_dir)

    for test in tests
      print "Running #{test}..."
      res = `qemu-arm-static -L /usr/arm-linux-gnueabi ./check_#{test} 2>&1`
      if $? == 0
        print "[ok]\n"
        @passed += 1
      else
        print "error\n\n", res, "\n"
        @failed += 1
      end
    end

    Dir.chdir(orig_dir)
  end
end

class TestTarget
  attr_reader :name
  def initialize(tests, conf_file)

    @tests = tests
    @skip_tests = {}

    @passed_tests = 0
    @failed_tests = 0
    @skipped_tests = 0

    # chop off '.conf' and use as name
    @name = conf_file[0..-(".conf".length + 1)]

    # get root element
    root = Document.new(File.new(conf_file)).root
    if root.name != "test-target"
      fail "expected <test-target> as root element, got #{root.name}"
    end

    # get specified arch
    @arch = root.get_text("arch").to_s
    if @arch == nil
      fail "no architecture specified, check <arch> element"
    end

    if !(@arch == "x86" || @arch == "arm")
      fail "unsupported architecture #{@arch}, must be either x86 or arm"
    end

    # get runner type
    runner_type = root.get_text("runner/type").to_s
    if runner_type == nil
      fail "no runner type specified, check <runner> -> <type> element"
    end

    @runner =
      case runner_type
        when "local" then LocalRunner.new(@arch)
        when "qemu-user" then QemuUserRunner.new(@arch)
        else fail "invalid runner type '#{runner_type}' specified"
      end

    # skip-list
    for test in root.get_elements("skip/*")
      @skip_tests[test.get_text.to_s] = true
    end
  end

  #
  # req_test the test to run, if nil/not specified run all tests
  #
  def run_tests(req_test = nil)
    if req_test != nil
      requested_tests = [req_test]
    else
      requested_tests = @tests
    end

    compiled_tests = []

    source_dir = Dir.pwd
    build_dir = make_build_dir

    # compile test binaries
    Dir.chdir(build_dir)
    for test in requested_tests

      # ignore tests on the skip list
      if @skip_tests[test]
        print "Skipping  #{test} test\n"
        @skipped_tests += 1
        next
      end

      compile_command = get_compile_command(source_dir, test)
      print "Compiling #{test}..."
      res = `#{compile_command}`

      if $? == 0
        print "[ok]\n"
        compiled_tests += [test]
      else
        @failed_tests += 1
        print "error\n", res
      end
    end
    Dir.chdir(source_dir)

    # run test that were successfully compiled
    @runner.run(build_dir, compiled_tests)
    @passed_tests += @runner.passed
    @failed_tests += @runner.failed
  end

  def make_build_dir
    Dir.mkdir(BuildDirRoot) unless File.exists?(BuildDirRoot)
    build_dir = BuildDirRoot + "/#{@name}"
    Dir.mkdir(build_dir) unless File.exists?(build_dir)
    build_dir
  end

  def get_compile_command(source_dir, test)
    cc = nil

    case @arch
      when "arm"
        cc="arm-linux-gnueabi-gcc"
        cflags=""
      when "x86"
        cflags="-m32 -lgc"
      else
        fail "unexpected architecture '#{@arch}'"
    end

    "make VPATH=#{source_dir} " +
         (cc != nil ? "CC=#{cc} " : "") +
         "XDC=#{source_dir}/../xdc " +
         "CFLAGS=\"-g #{cflags}\" " +
         "DFLAGS=\"--march=#{@arch} -I#{source_dir}\" " +
         "-f #{source_dir}/Makefile check_#{test} 2>&1"
  end

  def get_summary
   tests_run = @tests.length - @skipped_tests
   "#{tests_run} TEST SUITES RUN" +
   (@skipped_tests > 0 ? " (#{@skipped_tests} SKIPPED)" : "") +
   ", #{@passed_tests} PASSED " +
   "#{@failed_tests} FAILED"
  end
end

# load tests list
tests = File.new("tests.list").read.split

# load test target configurations
test_targets = {}
for conf in Dir["*.conf"]
  target = TestTarget.new(tests, conf)
  test_targets[target.name] = target
end

# parse command line arguments
OptionParser.new do |opts|
  opts.banner =
  "Usage: run_tests.rb [options] [tests]\n" +
  "   Run the specified tests. If no tests are specified, all tests are run.\n"

  opts.on("-l", "--list", "List available tests") do |l|
    for target_name in test_targets.keys
      for test_name in tests
        print "#{target_name}.#{test_name}\n"
      end
    end
    exit
  end
end.parse!

if ARGV.size > 0
  # if any test are specified as arguments,
  # run only them
  for arg in ARGV

    # figure out test target and test name
    parts = arg.split(".")
    if !(1..2).member?(parts.length) or parts[0].empty?
      fail "invalid test name '#{name}'"
    end

    # fetch the test target instance
    target = test_targets[parts[0]]
    if target == nil
      fail "unknown test target #{parts[0]}"
    end

    # run the requested test,
    # or all tests for the target if no name is specified
    target.run_tests(parts[1])

  end
else
  # run all tests
  test_targets.each_value { |t|
    print "Running tests for '", t.name,  "'\n"
    t.run_tests
  }

  # print test results for each target
  test_targets.each_value { |t| print t.name, ": ", t.get_summary, "\n" }
end


