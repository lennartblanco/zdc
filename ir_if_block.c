#include "ir_if_block.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ir_if_block_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrIfBlockClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrIfBlock),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrIfBlockType",
                                    &info, 0);
    }
    return type;
}

IrIfBlock *
ir_if_block_new(IrExpression *condition, IrCodeBlock *body)
{
    assert(condition);

    IrIfBlock *obj;

    obj = g_object_new(IR_TYPE_IF_BLOCK, NULL);
    
    obj->condition = condition;
    obj->body = body;

    return obj;
}

void
ir_if_block_set_condition(IrIfBlock *self, IrExpression *condition)
{
    assert(self);
    assert(IR_IS_IF_BLOCK(self));
    assert(condition);
    assert(IR_IS_EXPRESSION(condition));

    self->condition = condition;
}

IrExpression *
ir_if_block_get_condition(IrIfBlock *self)
{
    assert(self);
    assert(IR_IS_IF_BLOCK(self));

    return self->condition;
}

IrCodeBlock *
ir_if_block_get_body(IrIfBlock *self)
{
    assert(self);
    assert(IR_IS_IF_BLOCK(self));

    return self->body;
}


void
ir_if_block_print(IrIfBlock *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_IF_BLOCK(self));
    assert(out);

    fprintf_indent(out, indention, 
                   "if-block [%p]:\n condition: ", self);
    ast_node_print(XDP_AST_NODE(self->condition), out);
    fprintf(out, "\n");
    fprintf_indent(out, indention, " body:\n");
    ir_code_block_print(self->body, out, indention + 2);
}
