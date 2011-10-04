import std.stdio;
import std.conv;
import iml_operation;
import iml_variable;

enum TEMP_REG1 = "ecx";
enum TEMP_REG1_WORD = "cx";
enum TEMP_REG1_BYTE = "cl";

enum TEMP_REG2 = "eax";
enum TEMP_REG2_WORD = "ax";
enum TEMP_REG2_BYTE = "al";


/*---------------------------------------------------------------------------*
 *                   functions imported from x86.c                           *
 *---------------------------------------------------------------------------*/

extern (C) void
x86_move_from_reg(FILE *output, const char *src_reg, ImlVariable *var);

extern (C) void
x86_move_to_reg(FILE *output, const char *dest_reg, ImlOperand *oper);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

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

extern (C) void
x86_compile_sigext(FILE *output, iml_operation *op)
{
    /**
     * Get register's name for lowers 8 or 16 bits. E.g. 'eax' lowers 8 bits
     * are accessed as 'al'.
     *
     * @param get_8bit if true, return 8-bit name, otherwise return 16-bit name
     *
     * @return lower name or null if the register's lower bits not accessable
     */
    string get_reg_low_name(string register, bool get_8bit)
    {
        string res;
        switch (register)
        {
            case "eax":
                res = get_8bit ? "al" : "ax";
                break;
            case "ebx":
                res = get_8bit ? "bl" : "bx";
                break;
            case "ecx":
                res = get_8bit ? "cl" : "cx";
                break;
            case "edx":
                res = get_8bit ? "dl" : "dx";
                break;
            case "ebp":
                res = get_8bit ? null : "bp";
                break;
            case "esi":
                res = get_8bit ? null : "si";
                break;
            case "edi":
                res = get_8bit ? null : "di";
                break;
            default:
                assert(false, "unexpected register '" ~ register ~ "'");
        }
        return res;
    }

    assert(iml_operation_get_opcode(op) == iml_opcode.sigext);

    File asmfile = File.wrapFile(output);
    ImlVariable *val = cast(ImlVariable*)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 2);
    assert(iml_is_variable(val));
    assert(iml_is_variable(res));

    string val_suffix;
    switch (iml_variable_get_data_type(val))
    {
        case iml_data_type._8b:
            val_suffix = "b";
            break;
        case iml_data_type._16b:
            val_suffix = "w";
            break;
        default:
            assert(false, "unexpected value operand data type");
    }

    string res_suffix;
    switch (iml_variable_get_data_type(res))
    {
        case iml_data_type._16b:
            res_suffix = "w";
            break;
        case iml_data_type._32b:
        case iml_data_type.ptr:
            res_suffix = "l";
            break;
        default:
            assert(false, "unexpected result operand data type");
    }

    string val_exp = get_variable_asm_expression(val);
    if (iml_variable_get_register(val) != null)
    {
        /*
         * if value is stored in a register,
         * check if it's low bits are accessable
         */
        string reg = get_reg_low_name(val_exp[1..$], val_suffix == "b");
        if (reg == null)
        {
            /*
             * value registers low bits not accessable,
             * use temp register instead
             */
            x86_move_to_reg(output, TEMP_REG1, cast(ImlOperand *)val);
            reg = val_suffix == "b" ? TEMP_REG1_BYTE : TEMP_REG1_WORD;
        }
        val_exp = "%" ~ reg;
    }

    string res_exp;
    string res_reg = to!string(iml_variable_get_register(res));
    if (res_reg == null)
    {
        /*
         * 'movs' result operand must be a register,
         * if result operand is stored in function frame,
         * use temporary variable
         */
        asmfile.writefln("    xor %%%s, %%%s", TEMP_REG2, TEMP_REG2);
        res_exp = res_suffix == "w" ? TEMP_REG2_WORD : TEMP_REG2;
    }
    else
    {
        /* get result's lower bits name */
        res_exp =
            res_suffix == "w" ? get_reg_low_name(res_reg, false) : res_reg;
        assert(res_exp != null);
    }

    /* perform sign extension */
    asmfile.writefln("    movs%s%s %s, %%%s",
                     val_suffix, res_suffix, val_exp, res_exp);
    if (res_reg == null)
    {
        /* store the result into func frame offset if needed */
        x86_move_from_reg(output, TEMP_REG2, res);
    }
}
