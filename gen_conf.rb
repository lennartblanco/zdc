#!/usr/bin/ruby

require "rexml/document"

class Config
  def initialize(filenames)
    @targets = []
    @import_paths = []

    if filenames.size < 1 then
      raise "no configure files specified"
    end

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
        when "import_path" then @import_paths.push(element.text)
        else fail "unexpected tag '<#{element.name}>'"
      end
    end
  end

  def dump
    print "config ([\n"
    @targets.each do | target |
      target.dump
    end
    print "])\n"
  end

  def to_s
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

begin
  print Config.new(ARGV)
rescue
  print "#{$0}: #{$!}\n"
  exit 1
end
