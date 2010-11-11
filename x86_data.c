#include "x86_data.h"

#include "types.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_char_constant.h"

#include <assert.h>

static void
gen_data_section_string(FILE *out,
                        basic_data_type_t exp_type,
                        IrExpression *exp)
{
    switch (exp_type)
    {
        case int_type:
            fprintf(out, "%d",
                    ir_int_constant_get_value(IR_INT_CONSTANT(exp)));
            break;
        case uint_type:
            fprintf(out, "%u",
                    ir_uint_constant_get_value(IR_UINT_CONSTANT(exp)));
            break;
        case bool_type:
            fprintf(out, "%d",
                    ir_bool_constant_get_value(IR_BOOL_CONSTANT(exp)));
            break;
        case char_type:
            fprintf(out, "%u",
                    ir_char_constant_get_value(IR_CHAR_CONSTANT(exp)));
            break;
        default:
            /* unexpected expression data type */
            assert(false);
    }
}

static void
gen_array_literal_data(x86_comp_params_t *params,
                       IrArrayLiteral *array_literal)
{
    assert(params);
    assert(IR_IS_ARRAY_LITERAL(array_literal));

    char *data_type_directive;
    DtStaticArrayType *array_type;
    DtDataType *element_type;
    basic_data_type_t element_basic_type;
    GSList *i;

    array_type =
        DT_STATIC_ARRAY_TYPE(
            ir_expression_get_data_type(IR_EXPRESSION(array_literal)));
    element_type = dt_static_array_type_get_data_type(array_type);

    /* only array literal over basic data types are expected here */
    assert(DT_IS_BASIC_TYPE(element_type));
    element_basic_type =
        dt_basic_type_get_data_type(DT_BASIC_TYPE(element_type));

    if (types_is_bool(element_type) ||
        types_is_char(element_type))
    {
        data_type_directive = "byte";
    }
    else if (types_is_int(element_type) ||
             types_is_uint(element_type))
    {
        data_type_directive = "int";
    }
    else
    {
        /* unexpected element type */
        assert(false);
    }

    /* write label and array data type */
    fprintf(params->out, "%s: .%s ",
            ir_array_literal_get_data_label(array_literal),
            data_type_directive);

    /* write value of each array element */
    for (i = ir_array_literal_get_values(array_literal);
         i != NULL;
         i = g_slist_next(i))
    {
        gen_data_section_string(params->out,
                                element_basic_type,
                                IR_EXPRESSION(i->data));
        if (g_slist_next(i) != NULL)
        {
            fprintf(params->out, ", ");
        }
    }
    fputc('\n', params->out);
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_data_section(x86_comp_params_t *params,
                     GSList *data_section_exprs)
{
    GSList *i;

    if (data_section_exprs == NULL)
    {
        /* empty data section, nothing to generate */
        return;
    }

    fprintf(params->out, "    .data\n");

    for (i = data_section_exprs; i != NULL; i = g_slist_next(i))
    {
        /* only array literals in data section supported */
        assert(IR_IS_ARRAY_LITERAL(i->data));
        gen_array_literal_data(params, IR_ARRAY_LITERAL(i->data));
    }
}

