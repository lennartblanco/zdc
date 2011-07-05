#include <stdbool.h>

#include "ir_code_block.h"
#include "ir_statment.h"
#include "utils.h"

#include <assert.h>

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
        NULL,  /* class_init */
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
    obj->local_vars = NULL;

    return obj;
}

sym_table_t *
ir_code_block_get_symbols(IrCodeBlock *self)
{
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
    assert(IR_IS_CODE_BLOCK(self));

    return self->statments;
}

int
ir_code_block_add_local_var(IrCodeBlock *self, IrVariable *var)
{
    assert(IR_IS_CODE_BLOCK(self));
    assert(IR_IS_VARIABLE(var));

    self->local_vars = g_slist_append(self->local_vars, var);

    return sym_table_add_symbol(self->symbols, IR_SYMBOL(var));
}

/**
 * Get a local variable in this code block, as a list of IrVariable objects.
 */
GSList *
ir_code_block_get_local_vars(IrCodeBlock *self)
{
    assert(IR_IS_CODE_BLOCK(self));

    return self->local_vars;
}
