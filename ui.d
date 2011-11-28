/**
 * Compiler's entry point module.
 * Handle command line arguments and run compilation
 * with required options on provided source files.
 */
import std.stdio;
import std.string;
import std.conv;
import ver;
import GSList;
import auxil;
import arm;
import targets;

enum compilation_stages
{
    compile_stage,             /* generate assembly file (.s) */
    assemble_stage,            /* genarate object file   (.o) */
    link_stage                 /* build elf binary            */
}

/**
 * Setting for compilation step.
 */
struct compilation_settings
{
    GSList              *import_paths;
    arch_backend         backend;
    bool                 print_ast;
    bool                 print_ir;
    bool                 print_iml;
}

struct command_options
{
    compilation_stages last_compilation_stage;
    string[] libs;
    string[] source_files;
    string[] object_files;
    string output_file;

    compilation_settings comp_settings;
}

class CommandOptionException : Exception
{
    int exit_code;
    this(string error_message, int exit_code)
    {
        super(error_message);
        this.exit_code = exit_code;
    }
}

extern (C) void g_type_init();
extern (C) int compile_file(const char* input_file,
                            const char* output_file,
                            compilation_settings settings);
extern (C) uint system(const char *command);

extern (C) void x86_init(arch_backend *backend);

/**
 * Create an object file (.o) from provided assembly file by invoking assembler.
 * Object file name is generated from assembly file name by replacing .s
 * extension with .o.
 *
 * @return created object file name or null if assembler returned with error
 */
string
assemble_file(string target_name, string object_file_name, string assembly_file)
{

    if (object_file_name == null)
    {
        object_file_name = assembly_file[0..$-2] ~ ".o";
    }

    string command = get_target_asm_command(target_name,
                                            assembly_file,
                                            object_file_name);

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
link_files(string target_name,
           string output_file,
           string[] libs,
           string[] object_files)
{
    string ofile = output_file;

    if (ofile == null)
    {
        ofile = object_files[0][0..$-2];
    }

    string command = get_target_link_command(target_name,
                                             libs,
                                             object_files,
                                             ofile);

    if (system(std.string.toStringz(command)) != 0)
    {
        return -1;
    }
    return 0;
}

/**
 * Get long name of this compiler, including version numner.
 */
string
get_long_name()
{
    return "zdc D compiler, version " ~ VERSION ~ ".";
}

/**
 * Get the usage help message for zdc to stdout.
 *
 * @param progname the command issued to invoke zdc binary
 */
string
get_usage_message(string progname)
{
    return get_long_name() ~ "\n\n"
           "usage: " ~ progname ~ " [OPTION]... [SOURCE_FILES]...\n"
           "\n"
           "Compile specified D source file(s).\n"
            "\n"
            "Options:\n"
            "  -o outfile         Place output in file outfile.\n"
            "  -S                 Generate assembly files only.\n"
            "  -c                 Compile only, do not link.\n"
            "  -Ipath             Where to look for imported modules. When\n"
            "                     multiple -I are specified, the paths are\n"
            "                     search from left to right.\n"
            "  -llib              Link generated binary to specified library.\n"
            "  --print-ast        Output Abstaract Syntax Tree for each module\n"
            "                     unit.\n"
            "  --print-ir         Output Intermediate Represantation of each\n"
            "                     module.\n"
            "  --print-iml        Output Itermidiate language code generated "
            "                     for each module.\n"
            "  --version          Print compiler's version\n"
            "  --help, -?, -h     Print this help message.";
}

/**
 * Parse the command line arguments.
 *
 * @throw CommandOptionException on errors parsing arguments
 */
void
parse_command_arguments(string[] args, ref command_options options)
{
    /* set default compile options */
    options.last_compilation_stage = compilation_stages.link_stage;
    options.comp_settings.print_ast = false;
    options.comp_settings.print_ir = false;
    options.comp_settings.print_iml = false;

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
                throw new CommandOptionException(get_usage_message(args[0]), 0);
            }
            else if (arg == "--version")
            {
                throw new CommandOptionException(get_long_name(), 0);
            }
            else if (arg[0..2] == "-I")
            {
                string path = arg["-I".length..$];

                if (path == "")
                {
                    /* no path is specified */
                    throw new
                      CommandOptionException("argument to -I is missing", -1);
                }

                /* add trailing '/' if needed */
                if (path[$-1] != '/')
                {
                    path ~= '/';
                }

                options.comp_settings.import_paths =
                    g_slist_prepend(options.comp_settings.import_paths,
                                    cast(void*)toStringz(path));
            }
            else if (arg[0..2] == "-l")
            {
                string lib = arg["-l".length..$];

                if (lib == "")
                {
                    throw new
                      CommandOptionException("argument to -l is missing", -1);
                }
                options.libs ~= [lib];
            }
            else if (arg == "-o")
            {
                if (i + 1 >= args.length)
                {
                    throw new
                      CommandOptionException("argument to -o is missing", -1);
                }
                options.output_file = args[i+1];
                i += 1;
            }
            else if (arg == "-S")
            {
                options.last_compilation_stage =
                    compilation_stages.compile_stage;
            }
            else if (arg == "-c")
            {
                options.last_compilation_stage =
                    compilation_stages.assemble_stage;
            }
            else if (arg == "--print-ast")
            {
                options.comp_settings.print_ast = true;
            }
            else if (arg == "--print-ir")
            {
                options.comp_settings.print_ir = true;
            }
            else if (arg == "--print-iml")
            {
                options.comp_settings.print_iml = true;
            }
            else
            {
                throw new
                    CommandOptionException("unrecognized option '" ~ arg ~ "'",
                                           -1);
            }
            continue;
        }
        /* should be a D source file name, which ends with '.d' */
        else if (arg.length < 2 || arg[arg.length-2..$] != ".d")
        {
            throw new
              CommandOptionException(args[0] ~
                                       ": error: '" ~ arg ~
                                       "' not a d source file.",
                                     -1);
        }

        options.source_files ~= arg;
    }

    /* put the import paths in 'left-to-right' order */
    options.comp_settings.import_paths =
        g_slist_reverse(options.comp_settings.import_paths);

    /* always look for imports in local directory */
    options.comp_settings.import_paths =
        g_slist_prepend(options.comp_settings.import_paths,
                        cast(void*)toStringz("./"));

    /* did we get any source input files ? */
    if (options.source_files.length < 1)
    {
        throw new CommandOptionException(args[0] ~ ": no input files", -1);
    }
}

int
main(string[] args)
{
    command_options options;
    string compiled_output_file;
    string assembled_output_file;

    /*
     * parse command line options
     */
    try
    {
        parse_command_arguments(args, options);
    }
    catch (CommandOptionException ex)
    {
        /*
         * error detected while parsing options,
         * print error message and exit
         */
        writefln("%s", ex.msg);
        return ex.exit_code;
    }

    string target = get_target_name(args[0]);

    /* make sure output_file (-o) option makes sense */
    if (options.output_file != null &&
        options.last_compilation_stage != compilation_stages.link_stage &&
        options.source_files.length > 1)
    {
        writefln("%s: cannot specify -o with -c or -S with multiple files",
                 args[0]);
        return -1;
    }

    if (options.output_file != null &&
        options.source_files.length == 1)
    {
        switch (options.last_compilation_stage)
        {
            case compilation_stages.compile_stage:
                compiled_output_file = options.output_file;
                break;
            case compilation_stages.assemble_stage:
                assembled_output_file = options.output_file;
                break;
            default:
                /* nop */
                break;
        }
    }

    /* init arch specific backend */
    switch (get_target_backend(target))
    {
        case backend.x86:
            x86_init(&options.comp_settings.backend);
            break;
        case backend.arm:
            arm_init(&options.comp_settings.backend);
            break;
        default:
            assert(false, "unexpected backend");
    }

    /* compile and assemble all specified source files */
    g_type_init();
    foreach (file; options.source_files)
    {
        string target_file;

        if (compiled_output_file == null)
        {
            /*
             * get compilation target file from source file name
             * replace '.d' with '.s'
             */
            target_file = file[0..file.length-2] ~ ".s";
        }
        else
        {
            target_file = compiled_output_file;
        }

        /* invoke compilation */
        int r = compile_file(std.string.toStringz(file), 
                             std.string.toStringz(target_file),
                             options.comp_settings);
        if (r != 0)
        {
            /* there were error compiling the file, bail out */
            return r;
        }

        /* unless -S flag is specified, assemble generated file */
        if (options.last_compilation_stage > compilation_stages.compile_stage)
        {
            string obj_file = assemble_file(target,
                                            assembled_output_file,
                                            target_file);
            if (obj_file == null)
            {
                writefln("error assembling '%s'", target_file);
                return -1;
            }
            options.object_files ~= obj_file;
        }
    }

    /* unless linkage stage is disabled, invoke linker on our object files */
    if (options.last_compilation_stage >= compilation_stages.link_stage)
    {
        int r;

        r = link_files(target,
                       options.output_file,
                       options.libs,
                       options.object_files);
        if (r != 0)
        {
            return r;
        }
    }

    return 0;
}
