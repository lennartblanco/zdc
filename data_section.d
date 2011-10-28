import std.stdio;
import std.conv;
import ir_literal;
import ir_array_literal;
import ir_struct_literal;
import ir_struct_member;
import ir_module;
import ir_expression;
import ir_basic_constant;
import dt_array;
import dt_basic;

private string
get_data_section_string(DtBasic *exp_type,
                        IrBasicConstant *exp)
{
    switch (dt_basic_get_data_type(exp_type))
    {
        case basic_data_type.bool_type:
            return to!string(cast(int)ir_basic_constant_get_bool(exp));
        case basic_data_type.char_type:
            return to!string(cast(int)ir_basic_constant_get_char(exp));
        case basic_data_type.byte_type:
            return to!string(ir_basic_constant_get_byte(exp));
        case basic_data_type.ubyte_type:
            return to!string(ir_basic_constant_get_ubyte(exp));
        case basic_data_type.short_type:
            return to!string(ir_basic_constant_get_short(exp));
        case basic_data_type.ushort_type:
            return to!string(ir_basic_constant_get_ushort(exp));
        case basic_data_type.int_type:
            return to!string(ir_basic_constant_get_int(exp));
        case basic_data_type.uint_type:
            return to!string(ir_basic_constant_get_uint(exp));
        default:
            assert(false, "unexpected data type");
    }
}

private string
get_type_directive(DtBasic *type)
{
    switch (dt_basic_get_data_type(type))
    {
        case basic_data_type.bool_type:
        case basic_data_type.char_type:
        case basic_data_type.byte_type:
        case basic_data_type.ubyte_type:
            return ".byte";
        case basic_data_type.ushort_type:
        case basic_data_type.short_type:
            return ".short";
        case basic_data_type.int_type:
        case basic_data_type.uint_type:
            return ".int";
        default:
            assert(false,
                   "unexpected data type " ~
                    to!string(dt_basic_get_data_type(type)));
    }
}

/**
 * Generate the padding bytes directive string.
 *
 * @param padding_bytes number of padding bytes
 */
private string
get_padding_string(uint padding_bytes)
{
    assert(padding_bytes > 0);

    string padding_string = ".byte ";
    for (uint i = 0; i < padding_bytes; i += 1)
    {
        padding_string ~= "0, ";
    }

    return padding_string[0..$-2]; /* chop off last ', ' */
}

private void
gen_array_literal_data(File output, IrArrayLiteral *array_literal)
{
    string data_type_directive;
    DtArray *array_type;
    DtDataType *element_type;
    basic_data_type element_basic_type;

    array_type =
        cast(DtArray*)
            ir_expression_get_data_type(cast(IrExpression*)array_literal);
    element_type = dt_array_get_element_type(array_type);

    /* only array literal over basic data types are expected here */
    assert(dt_is_basic(element_type));

    /* write array data type */
    output.writef("%s ", get_type_directive(cast(DtBasic*)element_type));

    /* write value of each array element */
    for (GSList *i = ir_array_literal_get_values(array_literal);
         i != null;
         i = i.next())
    {
        output.writef("%s%s",
                      get_data_section_string(cast(DtBasic*)element_type,
                                              cast(IrBasicConstant*)i.data),
                      i.next() != null ? ", " : "\n");
    }
}

private void
gen_struct_literal_data(File output, IrStructLiteral *struct_literal)
{
    GSList *i = ir_struct_literal_get_members(struct_literal);
    output.writeln();
    for (; i != null; i = i.next())
    {
        IrStructMember *member = cast(IrStructMember *)i.data;
        IrExpression *init = ir_struct_member_get_init(member);
        DtDataType *type =
            ir_expression_get_data_type(cast(IrExpression*)member);
        assert(dt_is_basic(type), "only basic types in structs supported");

        output.writefln("  %s %s",
                        get_type_directive(cast(DtBasic*)type),
                        get_data_section_string(cast(DtBasic*)type,
                                                cast(IrBasicConstant*)init));

        /* insert padding zero bytes if needed */
        int padding;
        if ((padding = ir_struct_member_get_padding(member)) > 0)
        {
            output.writefln("  %s", get_padding_string(padding));
        }
    }
}

/**
 * C-wrapper around add_literals()
 */
extern (C) void
data_section_add_literals(FILE *output, IrModule *ir_module)
{
    add_literals(File.wrapFile(output), ir_module);
}

/**
 * Add literal module's literal expressions to .data section.
 */
void
add_literals(File output, IrModule *ir_module)
{
    GList *i;
    GList *data_section = ir_module_get_data_section(ir_module);

    if (data_section != null)
    {
        output.writefln("    .data");
    }

    for (i = data_section; i != null; i = i.next())
    {
        /* output literal's label */
        output.writef("%s: ",
                      to!string(
                      ir_literal_get_data_label(cast(IrLiteral*)i.data)));

        /* output literal's data */
        if (ir_is_array_literal(i.data))
        {
            gen_array_literal_data(output, cast(IrArrayLiteral*)i.data);
        }
        else if (ir_is_struct_literal(i.data))
        {
            gen_struct_literal_data(output, cast(IrStructLiteral*)i.data);
        }
        else
        {
            assert(false, "unexpected literal type in .data section");
        }
    }
    g_list_free(data_section);
}
