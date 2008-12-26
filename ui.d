/**
 * 
 */
import std.stdio;
import std.string;

extern (C) void g_type_init();
extern (C) int compile_file(char* input_file, char* output_file);

int main(char[][] args)
{
    /* did we get any source input files ? */
    if (args.length < 2)
    {
        writefln("%s: no input files", args[0]);
        return -1;
    }

    g_type_init();
    foreach (arg; args[1..$])
    {
        string target_file;

        /* check if the file name ends with '.d' */
        if(arg[arg.length-2..$] != ".d")
        {
            writefln("%s: error: '%s' not a d source file.", args[0], arg);
            return -1;
        }

        /* replace '.d' with '.j' for the target file name */
        target_file = arg[0..arg.length-2] ~ ".j";

        /* invoke compilation */
        int r = compile_file(std.string.toStringz(arg), 
                             std.string.toStringz(target_file));
        if (r != 0)
        {
            /* there were error compiling the file, bail out */
            return r;
        }
    }

    return 0;
}
