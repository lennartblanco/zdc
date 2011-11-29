#include "ir_if_else.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_if_else_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrIfElseClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrIfElse),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrIfElseType",
                                    &info, 0);
    }
    return type;
}

IrIfElse *
ir_if_else_new(void)
{
    IrIfElse *obj;

    obj = g_object_new(IR_TYPE_IF_ELSE, NULL);

    obj->if_else_blocks = NULL;
    obj->else_body = NULL;

    return obj;
}

void
ir_if_else_add_if_else_block(IrIfElse *self, IrIfBlock *if_block)
{
    assert(IR_IS_IF_ELSE(self));
    assert(IR_IS_IF_BLOCK(if_block));

    self->if_else_blocks = g_slist_append(self->if_else_blocks, if_block);
}

GSList *
ir_if_else_get_if_else_blocks(IrIfElse *self)
{
    assert(IR_IS_IF_ELSE(self));

    return self->if_else_blocks;
}

void
ir_if_else_set_else_body(IrIfElse *self, IrCodeBlock *else_body)
{
    assert(IR_IS_IF_ELSE(self));
    assert(IR_IS_CODE_BLOCK(else_body));

    self->else_body = else_body;
}

IrCodeBlock *
ir_if_else_get_else_body(IrIfElse *self)
{
    assert(IR_IS_IF_ELSE(self));

    return self->else_body;
}
