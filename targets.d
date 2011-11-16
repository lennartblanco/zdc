import std.string;

public:

enum backend
{
    x86,
    arm
}

string
get_target_name(string program_name)
{
    auto chop = lastIndexOf(program_name, '/');

    chop = (chop == -1) ? 0 : chop + 1;

    return program_name[chop..$];
}

backend
get_target_backend(string target_name)
{
    return get_target(target_name)._backend;
}

string
get_target_asm_command(string target_name,
                       string assembly_file,
                       string obj_file)
{
    return get_target(target_name)._toolchain.asm_cmd ~
           " " ~ obj_file ~ " " ~ assembly_file;
}

string
get_target_link_command(string target_name,
                        string[] obj_files,
                        string output_file)
{
    linker linker = get_target(target_name)._toolchain._linker;

    string command =
        linker.prefix ~ " " ~ linker.output_flag ~ " " ~ output_file;

    foreach (obj; obj_files)
    {
        command ~= " " ~ obj;
    }

    return command;
}

private:

struct target
{
    backend _backend;
    toolchain _toolchain;
}

struct toolchain
{
    string asm_cmd;
    linker _linker;
}

struct linker
{
    string prefix;
    string output_flag;
}

ref target
get_target(string target_name)
{
    static target[string] targets;

    if (targets == null)
    {
        targets = mixin(import("config"));
    }

    return targets[target_name];
}
