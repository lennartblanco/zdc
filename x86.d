import std.stdio;
import std.conv;
import iml_operation;
import iml_variable;

enum TEMP_REG1 = "ecx";
enum TEMP_REG1_BYTE = "cl";

extern (C) void
x86_move_from_reg(FILE *output, const char *src_reg, ImlVariable *var);

/**
 *  derive value assembly expression
 */
private string
get_variable_asm_expression(ImlVariable *var)
{
    string var_exp = to!string(iml_variable_get_register(var));
    if (var_exp != null)
    {
        /* value variable is stored in register */
        var_exp = "%" ~ var_exp;
    }
    else
    {
        /* value variable is stored in the function frame */
        var_exp = to!string(iml_variable_get_frame_offset(var)) ~ "(%ebp)";
    }

    return var_exp;
}

extern (C) void
x86_compile_bconv(FILE *output, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.bconv);

    File asmfile = File.wrapFile(output);
    ImlVariable *val = cast(ImlVariable*)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 2);
    assert(iml_is_variable(val));
    assert(iml_is_variable(res));

    string val_exp = get_variable_asm_expression(val);
    string res_exp = get_variable_asm_expression(res);

    asmfile.writefln("    xor %%" ~ TEMP_REG1 ~ ", %%" ~ TEMP_REG1 ~ "\n"
                     "    cmpl $0, %s\n"
                     "    setnz %%" ~ TEMP_REG1_BYTE ~ "\n"
                     "    movl %%"~ TEMP_REG1~ ", %s",
                     val_exp, res_exp);
}

extern (C) void
x86_compile_trunc(FILE *output, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.trunc);

    File asmfile = File.wrapFile(output);
    ImlVariable *val = cast(ImlVariable*)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 2);

    assert(iml_is_variable(val));
    assert(iml_is_variable(res));

    string val_exp = get_variable_asm_expression(val);
    string res_reg = to!string(iml_variable_get_register(res));
    if (res_reg == null)
    {
        res_reg = TEMP_REG1;
    }

    string bit_mask;
    switch (iml_variable_get_data_type(res))
    {
        case iml_data_type._8b:
            bit_mask = "0xff";
            break;
        case iml_data_type._16b:
            bit_mask = "0xffff";
            break;
        default:
            assert(false, "unexpected result data type");
    }

    asmfile.writefln("    movl $%s, %%%s\n"
                     "    and %s, %%%s",
                     bit_mask, res_reg, val_exp, res_reg);

    if (res_reg == TEMP_REG1)
    {
        x86_move_from_reg(output, TEMP_REG1, res);
    }
}