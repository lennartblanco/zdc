#ifndef IR_CODE_BLOCK_INC_X
#define IR_CODE_BLOCK_INC_X

#include <glib-object.h>

#include "sym_table.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_CODE_BLOCK ir_code_block_get_type()

#define IR_CODE_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_CODE_BLOCK, IrCodeBlock))

#define IR_CODE_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_CODE_BLOCK, IrCodeBlockClass))

#define IR_IS_CODE_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_CODE_BLOCK))

#define IR_IS_CODE_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_CODE_BLOCK))

#define IR_CODE_BLOCK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_CODE_BLOCK, IrCodeBlockClass))

typedef struct 
{
  GObject parent;
  /* private */
  sym_table_t *symbols;
  GSList      *statments;
} IrCodeBlock;

typedef struct 
{
  GObjectClass parent_class;
} IrCodeBlockClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_code_block_get_type(void);

IrCodeBlock *
ir_code_block_new(sym_table_t *parent_sym_table);

/**
 * Get the symbol table for this code block.
 */
sym_table_t *
ir_code_block_get_symbols(IrCodeBlock *self);

/**
 * Append a statment to the end of this code block.
 *
 * @param self the code block to append to
 * @param statment a statment to append, must be an instance of either
 *                  IR_STATMENT or XDP_AST_STATMENT
 */
void
ir_code_block_add_statment(IrCodeBlock *self, void *statment);

GSList *
ir_code_block_get_statments(IrCodeBlock *self);

void
ir_code_block_print(IrCodeBlock *self, FILE *out, int indention);

#endif /* IR_CODE_BLOCK_INC_X */
