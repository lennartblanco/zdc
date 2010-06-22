import std.stdio;
import std.c.stdio;
import std.c.string;

string
d_str(const char *c_str)
{
    char[] str = new char[strlen(c_str)];

    memcpy(str.ptr, c_str, str.length);

    return cast(string)str;
}

extern (C) void
arm_gen_code(void *ir_module, FILE *out_stream, const char *source_file)
{
    File f = File.wrapFile(out_stream);
    f.writefln("    .file \"%s\"", d_str(source_file));
    assert(false, "not implemented");
}
