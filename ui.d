/**
 * 
 */
import std.stdio;
import std.string;

struct compile_options_s
{
    bool print_ast;
};

extern (C) void g_type_init();
extern (C) int compile_file(char* input_file,
                            char* output_file,
                            compile_options_s options);
extern (C) int printf(char *str, ...);

int main(char[][] args)
{
    char[][] source_files;

    compile_options_s options;
    options.print_ast = false;

    /* parse command line options */
    foreach (arg; args[1..$])
    {
        /* should be an option */ 
        if (arg[0..2] == "--")
        {
            if (arg == "--print-ast")
            {
                options.print_ast = true;
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

        /* replace '.d' with '.j' for the target file name */
        target_file = file[0..file.length-2] ~ ".j";   

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
