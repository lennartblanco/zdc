#!/usr/bin/ruby

require "rexml/document"

class Config
  def initialize(mode, filenames)
    @mode = mode
    @targets = []
    @import_paths = []

    filenames.each do | file |
      parse_config_file(file)
    end
  end

  def parse_config_file(file_name)
    file = File.new(file_name)

    # silenty ignore empty files
    if file.stat.size == 0 then
      return
    end

    REXML::Document.new(file).root.elements.each do | element |
      case element.name
        when "target" then @targets.push(Target.new(element))
        when "import_path" then @import_paths.push(add_slash(element.text))
        else fail "unexpected tag '<#{element.name}>'"
      end
    end
  end

  # make sure the path ends with "/"
  def add_slash(path)
    path + if path[-1..-1] == "/" then "" else "/" end
  end

  def to_s
    if @mode == "targets" then
      names = ""
      @targets.each do | target |
         names += target.name + " "
      end
      return names[0..names.size-2]
    end
    "config\n([\n" +
      @targets.join(",\n") +
      "\n],\n[\n    \"" +
      @import_paths.join("\",\n    \"") + "\"\n])\n"
  end
end

class Target
  def initialize(target_element)
     target_element.elements.each do | element |
       case element.name
         when "name" then @name = element.text
         when "backend" then @backend = element.text
         when "toolchain" then @toolchain = Toolchain.new(element)
         else fail "unexpected tag '<#{element.name}>'"
       end
    end

  def name
    @name
  end

  def to_s
    "  \"#{@name}\" :\n" +
    "    target(backend.#{@backend},\n" +
    "    #{@toolchain})"
  end

  end
end

class Toolchain
  def initialize(toolchain_element)
     toolchain_element.elements.each do | element |
       case element.name
         when "asm_command" then @asm_command = element.text
         when "linker" then @linker = Linker.new(element)
         else fail "unexpected tag '<#{element.name}>'"
       end
     end
  end

  def to_s
    "toolchain(\"#{@asm_command}\",\n" +
    "      #{@linker})"
  end
end

class Linker
  def initialize(linker_element)
     linker_element.elements.each do | element |
       case element.name
         when "prefix" then @prefix = element.text
         when "link_flag" then @link_flag = element.text
         when "output_flag" then @output_flag = element.text
         else fail "unexpected tag '<#{element.name}>'"
       end
     end
  end

  def to_s
    "linker(\"#{@prefix}\",\n" +
     "             \"#{@link_flag}\",\n" +
     "             \"#{@output_flag}\")"
  end
end

def get_help_test
  "usage: #{$0} [--targets] CONFIG_FILE [CONFIG_FILE...]\n" +
  "  --targets only print target names"
end

begin
  mode = "mixin"
  files = []

  if ARGV.size == 0 then
    raise "no arguments\n" + get_help_test
  end

  ARGV.each do | arg |
    case arg
      when "--targets" then mode = "targets"
      when "--help"
        print get_help_test + "\n"
        exit 0
      else files.push(arg)
    end
  end

  if files.size == 0 then
    raise "no configure files specified"
  end

  print Config.new(mode, files)
rescue
  print "#{$0}: #{$!}\n"
  exit 1
end
