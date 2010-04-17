#include <stdbool.h>

#include "ir_code_block.h"
#include "ir_statment.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_code_block_class_init(gpointer klass, gpointer foo);

static void
ir_code_block_do_print(IrStatment *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_code_block_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrCodeBlockClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_code_block_class_init,  /* class_init */
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
ir_code_block_add_statment(IrCodeBlock *self, IrStatment *statment)
{
    assert(IR_IS_CODE_BLOCK(self));
    assert(IR_IS_STATMENT(statment));

    self->statments = g_slist_append(self->statments, statment);
}

GSList *
ir_code_block_get_statments(IrCodeBlock *self)
{
    assert(self);
    assert(IR_IS_CODE_BLOCK(self));

    return self->statments;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_code_block_class_init(gpointer klass, gpointer foo)
{
    ((IrStatmentClass *)klass)->do_print = ir_code_block_do_print;
}

static void
ir_code_block_do_print(IrStatment *self, FILE *out, int indention)
{
    assert(IR_IS_CODE_BLOCK(self));
    assert(out);

    IrCodeBlock *code_blk;

    code_blk = IR_CODE_BLOCK(self);
    GSList *i = code_blk->statments;
    GList *p;

    fprintf_indent(out, indention, "code block [%p]\n{\n", code_blk);

    /* print local symbols */
    p = sym_table_get_all_symbols(code_blk->symbols);
    for (; p != NULL; p = g_list_next(p))
    {
        ir_symbol_print(IR_SYMBOL(p->data), out, indention + 2);
    }
    g_list_free(p);

    /* print all statments in this code block */
    for (; i != NULL; i = g_slist_next(i))
    {
        ir_statment_print(i->data, out, indention + 2);
    }
    fprintf_indent(out, indention, "}\n");
}
