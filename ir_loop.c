#include "ir_loop.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_loop_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrLoopClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrLoop),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrLoopType",
                                    &info, 0);
    }
    return type;
}

void
ir_loop_set_exit_label(IrLoop *self, iml_operation_t *label)
{
    assert(IR_IS_LOOP(self));

    self->end_label = label;
}

iml_operation_t *
ir_loop_get_exit_label(IrLoop *self)
{
    assert(IR_IS_LOOP(self));
    assert(self->end_label);

    return self->end_label;
}
