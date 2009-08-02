#include <stdbool.h>

#include "ir_code_block.h"
#include "ir_statment.h"
#include "ast_node.h"
#include "ast_statment.h"
#include "utils.h"

#include <assert.h>
/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ir_code_block_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrCodeBlockClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrCodeBlock),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrCodeBlockType",
                                    &info, 0);
    }
    return type;
}

IrCodeBlock *
ir_code_block_new(sym_table_t *parent_sym_table)
{
    IrCodeBlock *obj;

    obj = g_object_new(IR_TYPE_CODE_BLOCK, NULL);
    obj->symbols = sym_table_new(parent_sym_table);

    return obj;
}

sym_table_t *
ir_code_block_get_symbols(IrCodeBlock *self)
{
    assert(self);
    assert(IR_IS_CODE_BLOCK(self));

    return self->symbols;
}

void
ir_code_block_add_statment(IrCodeBlock *self, void *statment)
{
    assert(self);
    assert(IR_IS_CODE_BLOCK(self));
    assert(statment);
    assert(IR_IS_STATMENT(statment) || 
           XDP_IS_AST_STATMENT(statment) ||
           IR_IS_CODE_BLOCK(statment));

    self->statments = g_slist_append(self->statments, statment);
}

GSList *
ir_code_block_get_statments(IrCodeBlock *self)
{
    assert(self);
    assert(IR_IS_CODE_BLOCK(self));

    return self->statments;
}

void
ir_code_block_print(IrCodeBlock *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_CODE_BLOCK(self));
    assert(out);

    GSList *i = self->statments;

    fprintf_indent(out, indention, "code block [%p]\n{\n", self);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (XDP_IS_AST_STATMENT(i->data))
        {
            ast_node_print(XDP_AST_NODE(i->data), out);
        }
        else if (IR_IS_STATMENT(i->data))
        {
            ir_statment_print(i->data, out, indention + 2);
        }
        else if (IR_IS_CODE_BLOCK(i->data))
        {
            ir_code_block_print(i->data, out, indention + 2);
        }
        else
        {
            /* unexpected type in the list */
            assert(false);
        }
    }
    fprintf_indent(out, indention, "}\n");
}
