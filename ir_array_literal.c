#include <stdbool.h>

#include "dt_basic.h"
#include "ir_array_literal.h"
#include "utils.h"

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
      type = g_type_register_static(IR_TYPE_LITERAL,
                                    "IrArrayLiteralType",
                                    &info, 0);
    }
    return type;
}

bool
ir_is_array_literal(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_TYPE((obj), IR_TYPE_ARRAY_LITERAL);
}

IrArrayLiteral *
ir_array_literal(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj),
                                      IR_TYPE_ARRAY_LITERAL,
                                      IrArrayLiteral);
}

IrArrayLiteral *
ir_array_literal_new(guint line_number)
{
    IrArrayLiteral *obj;

    obj = g_object_new(IR_TYPE_ARRAY_LITERAL,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->values = NULL;
    obj->data_type = NULL;
    obj->data_label = NULL;

    return obj;
}

void
ir_array_literal_add_value(IrArrayLiteral *self, IrExpression *value)
{
    assert(ir_is_array_literal(self));
    assert(IR_IS_EXPRESSION(value));

    self->values = g_slist_append(self->values, value);
}

void
ir_array_literal_set_values(IrArrayLiteral *self, GSList *values)
{
    assert(ir_is_array_literal(self));

    self->values = values;
    /* reset data type */
    self->data_type = NULL;
}

GSList *
ir_array_literal_get_values(IrArrayLiteral *self)
{
    assert(ir_is_array_literal(self));

    return self->values;
}

guint
ir_array_literal_get_length(IrArrayLiteral *self)
{
    assert(ir_is_array_literal(self));

    return g_slist_length(self->values);
}

guint
ir_array_literal_get_size(IrArrayLiteral *self)
{
    assert(ir_is_array_literal(self));

    return 
      dt_array_get_element_size(
        dt_array(
          ir_array_literal_do_get_data_type(ir_expression(self)))) *
                                                 g_slist_length(self->values);
           
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_array_literal_do_get_data_type(IrExpression *self)
{
    assert(ir_is_array_literal(self));

    IrArrayLiteral *arr_literal = ir_array_literal(self);
    if (arr_literal->data_type == NULL)
    {
        IrExpression *first_val;
        DtDataType *first_val_type;

        first_val = g_slist_nth_data(arr_literal->values, 0);
        assert(IR_IS_EXPRESSION(first_val));

        first_val_type = ir_expression_get_data_type(first_val);
        if (!dt_is_basic(first_val_type))
        {
            /* array literals of non-basic types not implemented */
            assert(false);
        }

        arr_literal->data_type = dt_array_new(first_val_type);
    }
    return DT_DATA_TYPE(arr_literal->data_type);
}

bool
ir_array_literal_do_is_constant(IrExpression *self)
{
    assert(ir_is_array_literal(self));

    GSList *i;

    /*
     * Check all literal value expression if they are compile-time constant
     */
    for (i = ir_array_literal(self)->values; i != NULL; i = g_slist_next(i))
    {
        if (!ir_expression_is_constant(ir_expression(i->data)))
        {
            /*
             * found an non-constant value expression, this array literal
             * expression is not compile-time constant
             */
            return false;
        }
    }

    /* all value expressions are compile-time constant */
    return true;
}

static void
ir_array_literal_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_array_literal_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->do_is_constant =
        ir_array_literal_do_is_constant;
}
