#include <stdbool.h>
#include <string.h>

#include "dt_basic.h"
#include "ir_array_literal.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_literal_class_init(gpointer klass, gpointer dummy);

static void
ir_array_literal_do_print(IrNode *self, FILE *out, int indention);

static DtDataType *
ir_array_literal_do_get_data_type(IrExpression *self);

bool
ir_array_literal_do_is_constant(IrExpression *self);

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

guint
ir_array_literal_get_length(IrArrayLiteral *self)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    return g_slist_length(self->values);
}

guint
ir_array_literal_get_size(IrArrayLiteral *self)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    return 
      dt_array_get_element_size(
        DT_ARRAY(
          ir_array_literal_do_get_data_type(IR_EXPRESSION(self)))) *
                                                 g_slist_length(self->values);
           
}

void
ir_array_literal_set_data_label(IrArrayLiteral *self, char *label)
{
    assert(IR_IS_ARRAY_LITERAL(self));
    assert(label || strlen(label) > 0);

    self->data_label = g_strdup(label);
}

char *
ir_array_literal_get_data_label(IrArrayLiteral *self)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    return self->data_label;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_literal_class_init(gpointer klass, gpointer dummy)
{
    ((IrNodeClass *)klass)->do_print = ir_array_literal_do_print;
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_literal_do_get_data_type;
    ((IrExpressionClass *)klass)->do_is_constant =
        ir_array_literal_do_is_constant;
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

        first_val = g_slist_nth_data(arr_literal->values, 0);
        assert(IR_IS_EXPRESSION(first_val));

        first_val_type = ir_expression_get_data_type(first_val);
        if (!DT_IS_BASIC(first_val_type))
        {
            /* array literals of non-basic types not implemented */
            assert(false);
        }

        arr_literal->data_type = dt_array_new(first_val_type);
    }
    return DT_DATA_TYPE(arr_literal->data_type);
}

static void
ir_array_literal_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_ARRAY_LITERAL(self));
    assert(out);

    GSList *i;

    fprintf_indent(out, indention, "[");
    for (i = IR_ARRAY_LITERAL(self)->values; i != NULL; i = g_slist_next(i))
    {
        ir_node_print(i->data, out, 0);
        if (g_slist_next(i) != NULL)
        {
            printf(", ");
        }
    }
    fprintf(out, "]\n");
}

bool
ir_array_literal_do_is_constant(IrExpression *self)
{
    assert(IR_IS_ARRAY_LITERAL(self));

    GSList *i;

    /*
     * Check all literal value expression if they are compile-time constant
     */
    for (i = IR_ARRAY_LITERAL(self)->values; i != NULL; i = g_slist_next(i))
    {
        if (!ir_expression_is_constant(IR_EXPRESSION(i->data)))
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
