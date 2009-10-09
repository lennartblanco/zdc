#include <stdbool.h>

#include "dt_basic_type.h"
#include "ir_array_literal.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_literal_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_array_literal_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_literal_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArrayLiteralClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_array_literal_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArrayLiteral),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrArrayLiteralType",
                                    &info, 0);
    }
    return type;
}

IrArrayLiteral *
ir_array_literal_new(gint32 value)
{
    IrArrayLiteral *obj;

    obj = g_object_new(IR_TYPE_ARRAY_LITERAL, NULL);
    obj->values = NULL;
    obj->data_type = NULL;

    return obj;
}

void
ir_array_literal_add_value(IrArrayLiteral *self, IrExpression *value)
{
    assert(IR_IS_ARRAY_LITERAL(self));
    assert(IR_IS_EXPRESSION(value));

    self->values = g_slist_append(self->values, value);
}

void
ir_array_literal_set_values(IrArrayLiteral *self, GSList *values)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    self->values = values;
    /* reset data type */
    self->data_type = NULL;
}

GSList *
ir_array_literal_get_values(IrArrayLiteral *self)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    return self->values;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_literal_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_literal_do_get_data_type;
}

static DtDataType *
ir_array_literal_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    IrArrayLiteral *arr_literal = IR_ARRAY_LITERAL(self);
    if (arr_literal->data_type == NULL)
    {
        IrExpression *first_val;
        DtDataType *first_val_type;
        basic_data_type_t first_val_basic_data_type;

        first_val = g_slist_nth_data(arr_literal->values, 0);
        assert(IR_IS_EXPRESSION(first_val));

        first_val_type = ir_expression_get_data_type(first_val);
        if (!DT_IS_BASIC_TYPE(first_val_type))
        {
            /* array literals of non-basic types not implemented */
            assert(false);
        }
        first_val_basic_data_type =
            dt_basic_type_get_data_type(DT_BASIC_TYPE(first_val_type));

        arr_literal->data_type =
            dt_static_array_type_new(first_val_basic_data_type,
                                     g_slist_length(arr_literal->values));
    }
    return DT_DATA_TYPE(arr_literal->data_type);
}
