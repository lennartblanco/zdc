#!/usr/bin/ruby

require "rexml/document"
include REXML

BuildDirRoot = "build"

class TestSet
  def initialize(tests, conf_file)

    @tests = tests

    # chop off '.conf' and use as name
    @name = conf_file[0..-(".conf".length + 1)]

    # get root element
    root = Document.new(File.new(conf_file)).root
    if root.name != "test-fixture"
      fail "expected <test-fixture> as root element, got #{root.name}"
    end

    # get specified arch
    @arch = root.get_text("arch")
    if @arch == nil
      fail "no architecture specified, check <arch> element"
    end

    if !(@arch == "x86" || @arch == "arm")
      fail "unsupported architecture #{@arch}, must be either x86 or arm"
    end

    # get runner type
    @runner_type = root.get_text("runner/type")
  end

  def compile
    source_dir = Dir.pwd
    Dir.chdir(make_build_dir)

    for test in @tests
      compile_command = get_compile_command(source_dir, test)
      print "Compiling #{test}..."
      res = `#{compile_command}`

      if $? == 0
        print "[ok]\n"
      else
        print "error\n", res
      end
    end
  end

  def make_build_dir
    Dir.mkdir(BuildDirRoot) unless File.exists?(BuildDirRoot)
    build_dir = BuildDirRoot + "/#{@name}"
    Dir.mkdir(build_dir) unless File.exists?(build_dir)
    build_dir
  end

  def get_compile_command(source_dir, test)
    "make VPATH=#{source_dir} XDC=#{source_dir}/../xdc " +
         "CFLAGS=\"-g -m32 -lgc\" DFLAGS=--march=#{@arch} " +
         "-f #{source_dir}/Makefile check_#{test}"
  end
end

# load tests list
tests = File.new("tests.list").read.split

# build and run tests
for conf in Dir["*.conf"]
  ts = TestSet.new(tests, conf)
  ts.compile
end


