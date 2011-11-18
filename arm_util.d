public import iml;

import std.conv;
import std.algorithm;

/**
 * generates a string of preserved registers that are used
 * in a function frame
 */
string
get_used_preserved_regs(iml_func_frame *func_frame)
{
    string preserved_regs = "";
    string[] regs;

    /*
     * the registers must be specified in accending order,
     * otherwise assembler will complain
     */

    for (auto i = iml_func_frame_get_used_regs(func_frame);
         i != null;
         i = i.next())
    {
        string reg = to!string(cast(char*)i.data);
        regs = reg ~ regs;
    }
    sort(regs);

    foreach (reg; regs)
    {
        preserved_regs ~= reg ~ ", ";
    }

    return preserved_regs;
}

string
get_blob_arg_regs(ref uint next_reg, ImlVariable *var)
{
    uint size = iml_variable_get_size(var);

    /* figure how many and which rX regs to use for storing the blob */
    uint r;
    string regs;
    for (r = next_reg; (r - next_reg) * 4 < size; r += 1)
    {
        regs ~= (regs != null ? ", " : "") ~ "r" ~ to!string(r);
    }
    next_reg = r;

    return regs;
}

string
mult_as_shift(uint mult)
{
    string shift_operand;

    switch (mult)
    {
        case 4:
            shift_operand = ", lsl #2";
            break;
        case 2:
            shift_operand = ", lsl #1";
            break;
        case 1:
            shift_operand = "";
            break;
        default:
            assert(false, "unsupported mult value");
    }

    return shift_operand;
}

/**
 * Figure out suitable suffix to ldr and str operations depending
 * on the size of the operand.
 */
string
access_operation_suffix(ImlOperand *operand)
{
    switch (iml_operand_get_data_type(operand))
    {
        case iml_data_type._32b:
        case iml_data_type.ptr:
            return "";
        case iml_data_type._16b:
            return "h";
        case iml_data_type._8b:
            return "b";
        default:
            assert(false, "unexpected operand data type");
    }
}

/**
 * Generate assembly to move an operand into specified register. The operand's
 * value can optionally be multiplied by an 2^x value.
 *
 * @param file where to write generated assembly
 * @param regiser register name where to store the operand
 * @param operand the operand to store in the register
 * @param mult if specified the value stored in register will be (operand * mult)
 *
 * @return the register were the operand have been moved, e.g. the value of
 *         register parameter
 */
string
gen_move_to_reg(File asmfile,
                string register,
                ImlOperand *operand,
                uint mult = 1)
{
    if (iml_is_constant(operand))
    {
        ImlConstant *const_op = cast(ImlConstant *)operand;
        if (iml_operand_get_data_type(operand) == iml_data_type.ptr)
        {
            assert(mult == 1); /* can't multiply label constants */
            asmfile.writefln("    ldr %s, =%s",
                             register,
                             to!string(iml_constant_get_val_ptr(const_op)));
        }
        else
        {
            asmfile.writefln("    ldr %s, =%s",
                             register,
                             iml_constant_get_val_32b(const_op) * mult);
        }
    }
    else
    {
        assert(iml_is_variable(operand));

        string shift_operand = mult_as_shift(mult);
        ImlVariable *var = cast(ImlVariable *)operand;
        char *reg = iml_variable_get_register(var);

        if (reg != null)
        {
            asmfile.writefln("    mov %s, %s%s",
                             register, to!string(reg), shift_operand);
        }
        else
        {
            asmfile.writefln("    ldr %s, [fp, #%s]",
                             register, iml_variable_get_frame_offset(var));
            if (shift_operand != "")
            {
                asmfile.writefln("    mov %s, %s%s",
                                 register, register, shift_operand);
            }
        }
    }

    return register;
}

void
gen_move_from_reg(File asmfile, string src_reg, ImlVariable *destination)
{
    char *dest_reg = iml_variable_get_register(destination);

    if (dest_reg != null)
    {
        asmfile.writefln("    mov %s, %s",
                         to!string(dest_reg),
                         src_reg);
    }
    else
    {
        asmfile.writefln("    str %s, [fp, #%s]",
                         src_reg,
                         iml_variable_get_frame_offset(destination));
    }
}

/**
 * Check if operand is stored in register. If not, generate assembly to
 * copy operand to provided register.
 *
 * @param asmfile the file where to write generated code
 * @param operand the operand to be copied to a register
 * @param register the register where operand can be stored
 * @param force move the operand to specified register even if it is already
 *              stored in a register
 *
 * @return the register where operand is stored
 */
string
store_in_reg(File asmfile,
             ImlOperand *operand,
             string register,
             bool force = false)
{
    if (iml_is_constant(operand))
    {
        gen_move_to_reg(asmfile, register, operand);
    }
    else
    {
        assert(iml_is_variable(operand));
        char *reg = iml_variable_get_register(cast(ImlVariable*)operand);
        if (reg == null || force)
        {
            gen_move_to_reg(asmfile, register, operand);
        }
        else
        {
            register = to!string(reg);
        }
    }

    return register;
}

/**
 * Generate the offset expression for ldr or str instruction.
 *
 * @param offset the IML offset value
 *
 * @return the offset expression or empty string if no offset or offset 0
 *         is specified
 */
string
get_offset_expression(ImlOperand *offset)
{
    assert(offset == null || iml_is_constant(offset),
           "only constant offsets implemented");
    ImlConstant *const_offset = cast(ImlConstant*)offset;

    if (offset == null || iml_constant_get_val_32b(const_offset) == 0)
    {
        return "";
    }

    return ", #" ~ to!string(iml_constant_get_val_32b(const_offset));
}

/**
 * Generate an array element address expression, which can be used in
 * ldr and str instructions. Will generate assembly to preform address
 * calculations if needed.
 *
 * @param asmfile  the file were to write generated assembly if needed
 * @param base the array base variable, must be of blob or ptr type
 * @param index the array index operand
 * @param size array element size in bytes
 * @param addr_reg the register to use as a part of address expression,
 *                 for cases when part of address expression must be stored
 *                 in a register, e.g. "fp, r0"
 * @param temp_reg temporary register to use for address calculations if needed
 *
 * @return the derived address expression, for example "fp, #-52"
 */
string
get_addr_exp(File asmfile,
             ImlVariable *base,
             ImlOperand *index,
             uint size,
             string addr_reg,
             string temp_reg)
{
    string addr_exp;

    if (iml_variable_get_data_type(base) == iml_data_type.blob)
    {
        int base_offset = iml_variable_get_frame_offset(base);

        if (iml_is_constant(index))
        {
            assert(iml_operand_get_data_type(index) == iml_data_type._32b);
            ImlConstant *const_idx = cast(ImlConstant *)index;
            addr_exp = "fp, #" ~
                to!string(base_offset +
                   cast(int)(iml_constant_get_val_32b(const_idx) * size));

        }
        else
        {
            assert(iml_is_variable(index));

            gen_move_to_reg(asmfile, addr_reg, index, size);
            asmfile.writefln("    add %s, %s, #%s",
                             addr_reg,
                             addr_reg,
                             base_offset);
            addr_exp = "fp, " ~ addr_reg;
        }
    }
    else
    {
        assert(iml_variable_get_data_type(base) == iml_data_type.ptr);

        string base_reg =
            store_in_reg(asmfile, cast(ImlOperand*)base, addr_reg);

        if (iml_is_constant(index))
        {
            ImlConstant *const_idx = cast(ImlConstant *)index;
            int offset = cast(int)iml_constant_get_val_32b(const_idx) * size;

            addr_exp = base_reg ~
                       (offset != 0 ? ", #" ~ to!string(offset) : "");
        }
        else
        {
            assert(iml_is_variable(index));

            ImlVariable *index_var = cast(ImlVariable *)index;
            string index_reg = store_in_reg(asmfile, index, temp_reg);

            asmfile.writefln("    add %s, %s, %s%s",
                             addr_reg,
                             base_reg,
                             index_reg,
                             mult_as_shift(size));
            addr_exp = addr_reg;
        }
    }

    return addr_exp;
}
