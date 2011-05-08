#!/usr/bin/ruby

require "rexml/document"
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

    @passed_tests = 0
    @failed_tests = 0
    @skiped_tests = 0

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
  end

  def run_tests

    compiled_tests = []

    source_dir = Dir.pwd
    build_dir = make_build_dir

    # compile test binaries
    Dir.chdir(build_dir)
    for test in @tests
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
   "#{@tests.length} TEST SUITES RUN, " +
   "#{@passed_tests} PASSED " +
   "#{@failed_tests} FAILED"
  end
end

# load tests list
tests = File.new("tests.list").read.split

# build and run tests
test_sets = []
for conf in Dir["*.conf"]
  target = TestTarget.new(tests, conf)
  print "Running tests for '", target.name,  "'\n"
  target.run_tests

  test_sets += [target]
end

# print test results for each target
test_sets.each { |t| print t.name, ": ", t.get_summary, "\n" }

