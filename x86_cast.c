#include <stdbool.h>

#include "x86.h"
#include "x86_cast.h"
#include "types.h"
#include "ast_basic_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_compile_cast_to_bool(IrExpression *value);

static void
x86_compile_cast_to_int(AstDataType *value_type);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_cast(x86_comp_params_t *params,
                 IrCast *cast,
                 sym_table_t *sym_table)
{
    IrExpression *value;
    AstDataType *target_type;

    value = ir_cast_get_value(cast);
    target_type = ir_cast_get_target_type(cast);

    x86_compile_expression(params, value, sym_table);

    if (!XDP_IS_AST_BASIC_TYPE(target_type))
    {
        /* not implemented */
        assert(false);
    }

    switch (ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(target_type)))
    {
        case bool_type:
            x86_compile_cast_to_bool(value);
            break;
        case int_type:
            x86_compile_cast_to_int(ir_expression_get_data_type(value));
            break;
        default:
            /* unexpected basic data type */
            assert(false);
    }
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
x86_compile_cast_to_bool(IrExpression *value)
{
    AstDataType *value_type;

    value_type = ir_expression_get_data_type(value);

    if (!XDP_IS_AST_BASIC_TYPE(value_type))
    {
        /* not implemented */
        assert(false);
    }

    if (types_is_literal_0or1(value))
    {
        /*
         * if literal int 0 or 1 is on the stack,
         * then no conversion is needed
         */
        return;
    }

    /* converting non-literal values to bool is not implemented */
    assert(false);
}

static void
x86_compile_cast_to_int(AstDataType *value_type)
{
    if (!XDP_IS_AST_BASIC_TYPE(value_type))
    {
        /* not implemented */
        assert(false);
    }

    switch (ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(value_type)))
    {
        case bool_type:
        case int_type:
            /* no need to explicitly convert boolean and integer values */
            break;
        default:
            /* unexpected basic data type */
            assert(false);
    }
}


