import std.stdio;
import ir_array_literal;
import ir_module;
import ir_expression;
import ir_basic_constant;
import dt_array;
import dt_basic;
import std.conv;

private void
gen_data_section_string(File output,
                        basic_data_type exp_type,
                        IrBasicConstant *exp)
{
    switch (exp_type)
    {
        case basic_data_type.int_type:
            output.writef("%s", ir_basic_constant_get_int(exp));
            break;
        case basic_data_type.uint_type:
            output.writef("%s", ir_basic_constant_get_uint(exp));
            break;
        case basic_data_type.bool_type:
            output.writef("%s", cast(int)ir_basic_constant_get_bool(exp));
            break;
        case basic_data_type.char_type:
            output.writef("%s", ir_basic_constant_get_char(exp));
            break;
        default:
            assert(false, "unexpected data type");
    }
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
    element_type = dt_array_get_data_type(array_type);

    /* only array literal over basic data types are expected here */
    assert(dt_is_basic(element_type));
    element_basic_type = dt_basic_get_data_type(cast(DtBasic*)element_type);

    switch (element_basic_type)
    {
        case basic_data_type.bool_type:
        case basic_data_type.char_type:
            data_type_directive = "byte";
            break;
        case basic_data_type.int_type:
        case basic_data_type.uint_type:
            data_type_directive = "int";
            break;
        default:
            assert(false, "unexpexted element type");
    }

    /* write label and array data type */
    output.writef("%s: .%s ",
                  to!string(ir_array_literal_get_data_label(array_literal)),
                  data_type_directive);

    /* write value of each array element */
    for (GSList *i = ir_array_literal_get_values(array_literal);
         i != null;
         i = i.next())
    {
        gen_data_section_string(output,
                                element_basic_type,
                                cast(IrBasicConstant*)i.data);
        if (i.next() != null)
        {
            output.write(", ");
        }
    }
    output.write("\n");
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
        assert(ir_is_array_literal(i.data),
               "only array literals in .data section supported");
        gen_array_literal_data(output, cast(IrArrayLiteral*)i.data);
    }
    g_list_free(data_section);
}
