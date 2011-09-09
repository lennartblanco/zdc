import std.stdio;
import std.conv;
import iml_operation;
import iml_variable;


enum TEMP_REG1 = "ecx";
enum TEMP_REG1_BYTE = "cl";

extern (C) void
x86_compile_bconv(FILE *output, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.bconv);
    assert(iml_is_variable(iml_operation_get_operand(op, 1)));
    assert(iml_is_variable(iml_operation_get_operand(op, 2)));

    File asmfile = File.wrapFile(output);
    ImlVariable *val = cast(ImlVariable*)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 2);

    /* derive value assembly expression */
    string val_exp = to!string(iml_variable_get_register(val));
    if (val_exp != null)
    {
        /* value variable is stored in register */
        val_exp = "%" ~ val_exp;
    }
    else
    {
        /* value variable is stored in the function frame */
        val_exp = to!string(iml_variable_get_frame_offset(val)) ~ "(%ebp)";
    }

    /* derive result assembly expression */
    string res_exp = to!string(iml_variable_get_register(res));
    if (res_exp != null)
    {
        /* result variable is stored in register */
        res_exp = "%" ~ res_exp;
    }
    else
    {
        /* result variable is stored in the function frame */
        res_exp = to!string(iml_variable_get_frame_offset(res)) ~ "(%ebp)";
    }

    asmfile.writefln("    xor %%" ~ TEMP_REG1 ~ ", %%" ~ TEMP_REG1 ~ "\n"
                     "    cmpl $0, %s\n"
                     "    setnz %%" ~ TEMP_REG1_BYTE ~ "\n"
                     "    movl %%"~ TEMP_REG1~ ", %s",
                     val_exp, res_exp);
}