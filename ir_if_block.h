#ifndef IR_IF_BLOCK_INC_X
#define IR_IF_BLOCK_INC_X

#include "ast_expression.h"
#include "ir_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_IF_BLOCK ir_if_block_get_type()

#define IR_IF_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_IF_BLOCK, IrIfBlock))

#define IR_IF_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_IF_BLOCK, IrIfBlockClass))

#define IR_IS_IF_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_IF_BLOCK))

#define IR_IS_IF_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_IF_BLOCK))

#define IR_IF_BLOCK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_IF_BLOCK, IrIfBlockClass))

typedef struct 
{
  GObject        parent;
  /* private */
  AstExpression *condition;
  IrCodeBlock   *body;
} IrIfBlock;

typedef struct 
{
  GObjectClass parent_class;
} IrIfBlockClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_if_block_get_type(void);

IrIfBlock *
ir_if_block_new(AstExpression *condition, IrCodeBlock *body);

AstExpression *
ir_if_block_get_condition(IrIfBlock *self);

IrCodeBlock *
ir_if_block_get_body(IrIfBlock *self);

void
ir_if_block_print(IrIfBlock *self, FILE *out, int indention);


#endif /* IR_IF_BLOCK_INC_X */