#include "ir_cast.h"
#include "dt_data_type.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_cast_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_cast_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrCastClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_cast_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrCast),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrCastType",
                                    &info, 0);
    }
    return type;
}

IrCast *
ir_cast(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_CAST, IrCast);
}

IrCast *
ir_cast_new(DtDataType *target_type, IrExpression *value, guint line_number)
{
    IrCast *obj;

    obj = g_object_new(IR_TYPE_CAST, "ir-node-line-number", line_number, NULL);
    obj->target_type = target_type;
    obj->value = value;

    return obj;
}

DtDataType *
ir_cast_get_target_type(IrCast *self)
{
    assert(IR_IS_CAST(self));

    return self->target_type;
}

void
ir_cast_set_value(IrCast *self, IrExpression *value)
{
    assert(IR_IS_CAST(self));
    assert(IR_IS_EXPRESSION(value));

    self->value = value;
}

IrExpression *
ir_cast_get_value(IrCast *self)
{
    assert(IR_IS_CAST(self));

    return self->value;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_cast_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_CAST(self));

    return ir_cast(self)->target_type;
}

bool
ir_cast_do_is_constant(IrExpression *self)
{
    assert(IR_IS_CAST(self));

    return ir_expression_is_constant(ir_cast(self)->value);
}

static void
ir_cast_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type = ir_cast_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->do_is_constant = ir_cast_do_is_constant;
}
