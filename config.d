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
                        string[] libs,
                        string[] obj_files,
                        string output_file)
{
    linker linker = get_target(target_name)._toolchain._linker;

    string command = linker.prefix;

    /* add link flags */
    foreach (lib; libs)
    {
        command ~= " " ~ linker.link_flag ~ lib;
    }

    /* add output file name option */
    command ~= " " ~ linker.output_flag ~ " " ~ output_file;

    /* add object files to link */
    foreach (obj; obj_files)
    {
        command ~= " " ~ obj;
    }

    return command;
}

string[]
get_targets_names()
{
    return get_config().targets.keys;
}

private:

struct config
{
    target[string] targets;
    string[] import_paths;
}

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
    string link_flag;
    string output_flag;
}

ref config
get_config()
{
    static config c;
    if (c.targets == null)
    {
        c = mixin(import("config.mixin"));
    }

    return c;
}

ref target
get_target(string target_name)
{
    return get_config().targets[target_name];
}
