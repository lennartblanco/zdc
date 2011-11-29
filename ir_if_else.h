#ifndef IR_IF_ELSE_INC_X
#define IR_IF_ELSE_INC_X

#include "ir_statment.h"
#include "ir_code_block.h"
#include "ir_if_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_IF_ELSE ir_if_else_get_type()

#define IR_IF_ELSE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_IF_ELSE, IrIfElse))

#define IR_IF_ELSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_IF_ELSE, IrIfElseClass))

#define IR_IS_IF_ELSE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_IF_ELSE))

#define IR_IS_IF_ELSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_IF_ELSE))

#define IR_IF_ELSE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_IF_ELSE, IrIfElseClass))

typedef struct
{
    IrStatment  parent;
    /* private */
    GSList      *if_else_blocks;
    IrCodeBlock *else_body;
} IrIfElse;

typedef struct
{
    IrStatmentClass parent_class;
} IrIfElseClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_if_else_get_type(void);

IrIfElse *
ir_if_else_new(void);

void
ir_if_else_add_if_else_block(IrIfElse *self, IrIfBlock *if_block);

/**
 * @return a list of if-blocks as an instances of IrIfBlock objects
 */
GSList *
ir_if_else_get_if_else_blocks(IrIfElse *self);

void
ir_if_else_set_else_body(IrIfElse *self, IrCodeBlock *else_body);

IrCodeBlock *
ir_if_else_get_else_body(IrIfElse *self);

#endif /* IR_IF_ELSE_INC_X */
