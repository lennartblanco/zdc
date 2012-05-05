#include "ir_new.h"
#include "dt_struct.h"
#include "dt_pointer.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_new_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_new_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrNewClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_new_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrNew),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrNewType",
                                    &info, 0);
    }
    return type;
}

IrNew *
ir_new_new(DtDataType *type, guint line_number)
{
    assert(DT_IS_DATA_TYPE(type));

    IrNew *obj;

    obj = g_object_new(IR_TYPE_NEW,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->type = type;

    return obj;
}

DtDataType *
ir_new_get_dt_type(IrNew *self)
{
    assert(IR_IS_NEW(self));

    return self->type;
}

void
ir_new_set_dt_type(IrNew *self, DtDataType *type)
{
    assert(IR_IS_NEW(self));
    assert(DT_IS_DATA_TYPE(type));

    self->type = type;
    self->exp_type = NULL;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_new_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_NEW(self));

    IrNew *new = IR_NEW(self);

    /* only new on struct types implemented */
    assert(DT_IS_STRUCT(new->type));
    if (new->exp_type == NULL)
    {
        new->exp_type = DT_DATA_TYPE(dt_pointer_new(new->type));
    }

    return new->exp_type;
}

static void
ir_new_class_init(gpointer klass, gpointer dummy)
{
    assert(IR_IS_NEW_CLASS(klass));

    IR_EXPRESSION_CLASS(klass)->do_get_data_type = ir_new_do_get_data_type;
}
