/**
 * Compiler's entry point module.
 * Handle command line arguments and run compilation
 * with required options on provided source files.
 */
import std.stdio;
import std.string;

enum arch_types_e
{
   arch_java = 0,
   arch_x86  = 1
}

enum compilation_stages_e
{
  compile_stage,             /* generate assembly file (.s) */
  assemble_stage,            /* genarate object file   (.o) */
  link_stage                 /* build elf binary            */
}

struct compile_options_s
{
    arch_types_e         target_arch;
    bool                 print_ast;
    bool                 print_ir;
};

extern (C) void g_type_init();
extern (C) int compile_file(const char* input_file,
                            const char* output_file,
                            compile_options_s options);
extern (C) uint system(const char *command);

/**
 * Print the usage help message for xdc to stdout.
 *
 * @param progname the command issued to invoke xdc binary
 */
void
print_usage_message(string progname)
{
    writefln("usage: %s [OPTION]... [SOURCE_FILES]...\n", progname);
    writefln("Compile specified D source file(s).\n"
             "\n"
             "Options:\n"
             "  --march=arch,      Generate code for the specified architecture.\n"
             "   or -march=arch    The choices for arch are 'java' and 'x86'.\n"
             "                     If 'java' is specified, compiler will emmit\n"
             "                     .j java assembly files. If 'x86' is specified,\n"
             "                     compiler will emmit .s x86 assembly files.\n"
             "  -o outfile         Place output in file outfile.\n"
             "  -S                 Generate assembly files only.\n"
             "  -c                 Compile only, do not link.\n"
             "  --print-ast        Output Abstaract Syntax Tree for each compile\n"
             "                     unit.\n"
             "  --print-ir         Output Intermediate Represantation of each\n"
             "                     compile unit.\n"
             "  --help, -?, -h     Print this help message."
             );
}

arch_types_e
parse_march_option(string option)
{
    string arch;

    int eqsign = indexOf(option, '=');

    if (eqsign > -1)
    {
        arch = option[eqsign + 1..$];
    }

    if (eqsign == -1 || arch.length <= 0)
    {
        throw new Exception("no value provided for '" ~ option ~
                            "' option");
    }

    switch (arch)
    {
        case "x86":
            return arch_types_e.arch_x86;
        case "java":
            return arch_types_e.arch_java;
        default:
            throw new Exception("unsupported target architecture '" ~ arch ~
                                "' specified");
            break;
    }
    /* we should not get here */
    assert(false);
}

string
get_target_file_name(string source_file, arch_types_e arch)
{
    string file_ext;
    switch (arch)
    {
        case arch_types_e.arch_java:
            file_ext = ".j";
            break;
        case arch_types_e.arch_x86:
            file_ext = ".s";
            break;
        default:
            /* unexpected architecture type */
            assert(false);
    }

    /* replace '.d' with target specific file extansion */
    return source_file[0..source_file.length-2] ~ file_ext;
}

/**
 * Create an object file (.o) from provided assembly file by invoking assembler.
 * Object file name is generated from assembly file name by replacing .s
 * extension with .o.
 *
 * @return created object file name or null if assembler returned with error
 */
string
assemble_file(string assembly_file)
{
    string object_file_name;
    string command;

    object_file_name = assembly_file[0..$-2] ~ ".o";

    command = "as --32 -g -o " ~ object_file_name ~ " " ~ assembly_file;

    if (system(std.string.toStringz(command)) != 0)
    {
        return null;
    }

  return object_file_name;
}

/**
 * Link the provided object files and libc into output elf file.
 *
 * @param output_file the elf output file. if null is provided, an output
 *                    file will be generated from first object file name.
 * @param object_files the object file names to link
 * @return 0 if linked successfully, -1 if linker returned an error
 */
int
link_files(string output_file, string[] object_files)
{
    string ofile = output_file;
    string command;
    string crt1_path;

    if (ofile == null)
    {
        ofile = object_files[0][0..$-2];
    }

    version (X86_64)
    {
        crt1_path = "/usr/lib32/";
    }
    version (X86)
    {
        crt1_path = "/usr/lib/";
    }

    command = "ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 " ~ 
               crt1_path ~ "crt1.o -o " ~ ofile ~ " -lc -lgc";
    foreach (file; object_files)
    {
      command ~= " " ~ file;
    }

    if (system(std.string.toStringz(command)) != 0)
    {
        return -1;
    }
    return 0;
}

int 
main(string[] args)
{
    string[] source_files;
    string[] object_files;
    string output_file;

    compile_options_s options;
    compilation_stages_e last_compilation_stage;

    /* set default compile options */
    last_compilation_stage = compilation_stages_e.link_stage;
    options.target_arch = arch_types_e.arch_x86;
    options.print_ast = false;
    options.print_ir = false;

    /* parse command line options */
    for (int i = 1; i < args.length; i += 1)
    {
        string arg = args[i];

        /* skip empty string arguments */
        if (arg.length <= 0)
        {
            continue;
        }

        /* should be an option */ 
        if (arg.length >= 2 &&
            (arg[0..2] == "--" || arg[0..1] == "-"))
        {
            if (arg == "--help" ||
                arg == "-?"     ||
                arg == "-h")
            {
                print_usage_message(args[0]);
                return 0;
            }
            else if (arg == "-o")
            {
                if (i + 1 >= args.length)
                {
                    writefln("bork");
                    return -1;
                }
                output_file = args[i+1];
                i += 1;
            }
            else if (arg == "-S")
            {
                last_compilation_stage = compilation_stages_e.compile_stage;
            }
            else if (arg == "-c")
            {
                last_compilation_stage = compilation_stages_e.assemble_stage;
            }
            else if (arg == "--print-ast")
            {
                options.print_ast = true;
            }
            else if (arg == "--print-ir")
            {
                options.print_ir = true;
            }
            else if ((arg.length >=6 && arg[0..6] == "-march") ||
                     (arg.length >=7 && arg[0..7] == "--march"))
            {
                options.target_arch = parse_march_option(arg);
            }
            else
            {
                writefln("unrecognized option '%s'", arg);
                return -1; 
            }
            continue;
        }
        /* should be a D source file name, which ends with '.d' */
        else if (arg.length < 2 || arg[arg.length-2..$] != ".d")
        {
            writefln("%s: error: '%s' not a d source file.", args[0], arg);
            return -1;
        }

        source_files ~= arg;
    }

    /* did we get any source input files ? */
    if (source_files.length < 1)
    {
        writefln("%s: no input files", args[0]);
        return -1;
    }

    /* compile and assemble all specified source files */
    g_type_init();
    foreach (file; source_files)
    {
        string target_file;

        /* get compilation target file from source file name */
        target_file = get_target_file_name(file, options.target_arch);

        /* invoke compilation */
        int r = compile_file(std.string.toStringz(file), 
                             std.string.toStringz(target_file),
                             options);
        if (r != 0)
        {
            /* there were error compiling the file, bail out */
            return r;
        }

        /* unless -S flag is specified, assemble generated file */
        if (last_compilation_stage > compilation_stages_e.compile_stage)
        {
            string obj_file = assemble_file(target_file);
            if (obj_file == null)
            {
                writefln("error assembling '%s'", target_file);
                return -1;
            }
            object_files ~= obj_file;
        }
    }

    /* unless linkage stage is disabled, invoke linker on our object files */
    if (last_compilation_stage >= compilation_stages_e.link_stage)
    {
        int r;

        r = link_files(output_file, object_files);
        if (r != 0)
        {
            return r;
        }
    }

    return 0;
}
