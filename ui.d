/**
 * Compiler's entry point module.
 * Handle command line arguments and run compilation on
 * with required options on provided source files.
 */
import std.stdio;
import std.string;

enum arch_types_e
{
   arch_java = 0,
   arch_x86  = 1
}

struct compile_options_s
{
    arch_types_e  target_arch;
    bool          print_ast;
    bool          print_ir;
};

extern (C) void g_type_init();
extern (C) int compile_file(char* input_file,
                            char* output_file,
                            compile_options_s options);

/**
 * Print the usage help message for xdc to stdout.
 *
 * @param progname the command issued to invoke xdc binary
 */
void
print_usage_message(char[] progname)
{
    writefln("usage: %s [OPTION]... [SOURCE_FILES]...\n", progname);
    writefln("Compile specified D source file(s).\n"
             "\n"
             "Options:\n"
             "  --march=arch,      Generate code for the specified architecture.\n"
             "   or -march=arch    The choices for arch are 'java' and 'x86'.\n"
             "                     If 'java' is specified, compiler will emmit\n"
             "                     .j java assembly files. If 'x86' is specified,\n"
             "                     compiler will emmit .S x86 assembly files.\n"
             "  --print-ast        Output Abstaract Syntax Tree for each compile\n"
             "                     unit.\n"
             "  --print-ir         Output Intermediate Represantation of each\n"
             "                     compile unit.\n"
             "  --help, -?, -h     Print this help message."
             );
}

arch_types_e
parse_march_option(char[] option)
{
    char[] arch;

    int eqsign = find(option, "=");

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

char[]
get_target_file_name(char[] source_file, arch_types_e arch)
{
    char[] file_ext;
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

int 
main(char[][] args)
{
    char[][] source_files;

    compile_options_s options;

    /* set default compile options */
    options.target_arch = arch_types_e.arch_x86;
    options.print_ast = false;
    options.print_ir = false;

    /* parse command line options */
    foreach (arg; args[1..$])
    {
        /* skip empty string arguments */
        if (arg.length <= 0)
        {
            continue;
        }

        /* should be an option */ 
        if (arg[0..2] == "--" || arg[0..1] == "-")
        {
            if (arg == "--help" ||
                arg == "-?"     ||
                arg == "-h")
            {
                print_usage_message(args[0]);
                return 0;
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
        else if (arg[arg.length-2..$] != ".d")
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

    /* compile all specified source files */
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
    }
    return 0;
}
