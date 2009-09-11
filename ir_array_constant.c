#include <stdbool.h>

#include "ast_basic_type.h"
#include "ir_array_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_constant_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_array_constant_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArrayConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_array_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArrayConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrArrayConstantType",
                                    &info, 0);
    }
    return type;
}

IrArrayConstant *
ir_array_constant_new(gint32 value)
{
    IrArrayConstant *obj;

    obj = g_object_new(IR_TYPE_ARRAY_CONSTANT, NULL);
    obj->values = NULL;
    obj->data_type = NULL;

    return obj;
}

void
ir_array_constant_add_value(IrArrayConstant *self, IrExpression *value)
{
    assert(IR_IS_ARRAY_CONSTANT(self));
    assert(IR_IS_EXPRESSION(value));

    self->values = g_slist_append(self->values, value);
}

void
ir_array_constant_set_values(IrArrayConstant *self, GSList *values)
{
    assert(IR_IS_ARRAY_CONSTANT(self));

    self->values = values;
    /* reset data type */
    self->data_type = NULL;
}

GSList *
ir_array_constant_get_values(IrArrayConstant *self)
{
    assert(IR_IS_ARRAY_CONSTANT(self));

    return self->values;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_constant_do_get_data_type;
}

static AstDataType *
ir_array_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ARRAY_CONSTANT(self));

    IrArrayConstant *arr_const = IR_ARRAY_CONSTANT(self);
    if (arr_const->data_type == NULL)
    {
        IrExpression *first_val;
        AstDataType *first_val_type;
        basic_data_type_t first_val_basic_data_type;

        first_val = g_slist_nth_data(arr_const->values, 0);
        assert(IR_IS_EXPRESSION(first_val));

        first_val_type = ir_expression_get_data_type(first_val);
        if (!XDP_IS_AST_BASIC_TYPE(first_val_type))
        {
            /* array literals of non-basic types not implemented */
            assert(false);
        }
        first_val_basic_data_type =
            ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(first_val_type));

        arr_const->data_type =
            ast_static_array_type_new(first_val_basic_data_type,
                                      g_slist_length(arr_const->values));
    }
    return XDP_AST_DATA_TYPE(arr_const->data_type);
}
