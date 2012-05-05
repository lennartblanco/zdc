#include "ir_null.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_null_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_null_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrNullClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_null_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrNull),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrNullType",
                                    &info, 0);
    }
    return type;
}

bool
ir_is_null(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_TYPE((obj), IR_TYPE_NULL);
}

IrNull *
ir_null_new(guint line_number)
{
    return g_object_new(IR_TYPE_NULL,
                        "ir-node-line-number", line_number,
                        NULL);
}

IrNull *
ir_null_get()
{
    static IrNull *ir_null = NULL;

    if (ir_null == NULL)
    {
        ir_null = ir_null_new(0);
    }

    return ir_null;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_null_do_get_data_type(IrExpression *self)
{
    assert(ir_is_null(self));

    return types_get_void_ptr();
}

static void
ir_null_class_init(gpointer klass, gpointer dummy)
{
    assert(IR_IS_NULL_CLASS(klass));

    IR_EXPRESSION_CLASS(klass)->do_get_data_type = ir_null_do_get_data_type;
}
