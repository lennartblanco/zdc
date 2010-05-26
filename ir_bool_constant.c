#include <stdbool.h>

#include "types.h"
#include "ir_bool_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_bool_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_bool_constant_do_get_data_type(IrExpression *self);

static void
ir_bool_constant_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_bool_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrBoolConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_bool_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrBoolConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrBoolConstantType",
                                    &info, 0);
    }
    return type;
}

IrBoolConstant *
ir_bool_constant_new(gboolean value, guint line_number)
{
    IrBoolConstant *obj;

    obj = g_object_new(IR_TYPE_BOOL_CONSTANT,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->value = value;

    return obj;
}

gboolean
ir_bool_constant_get_value(IrBoolConstant *self)
{
    assert(IR_IS_BOOL_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_bool_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_bool_constant_do_get_data_type;
    ((IrNodeClass *)klass)->do_print = ir_bool_constant_do_print;
}

static DtDataType *
ir_bool_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_BOOL_CONSTANT(self));

    return types_get_bool_type();
}

static void
ir_bool_constant_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_BOOL_CONSTANT(self));
    assert(out);

    fprintf(out, "%s", IR_BOOL_CONSTANT(self)->value ? "true" : "false");
}
