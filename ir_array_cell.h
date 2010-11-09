#ifndef IR_ARRAY_CELL_INC_X
#define IR_ARRAY_CELL_INC_X

#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY_CELL ir_array_cell_get_type()

#define IR_ARRAY_CELL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY_CELL, IrArrayCell))

#define IR_ARRAY_CELL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY_CELL, IrArrayCellClass))

#define IR_IS_ARRAY_CELL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ARRAY_CELL))

#define IR_IS_ARRAY_CELL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY_CELL))

#define IR_ARRAY_CELL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY_CELL, IrArrayCellClass))

typedef struct
{
    IrExpression parent;

    /* private */
    IrVariable   *array;
    IrExpression *index;
    DtDataType   *data_type;
} IrArrayCell;

typedef struct
{
    IrExpressionClass parent_class;
} IrArrayCellClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_cell_get_type(void);

IrArrayCell *
ir_array_cell_new(IrVariable *array, IrExpression *index, guint line_number);

/**
 * get array symbol
 */
IrVariable *
ir_array_cell_get_symbol(IrArrayCell *self);

/**
 * get array index expression
 */
IrExpression *
ir_array_cell_get_index(IrArrayCell *self);

void
ir_array_cell_set_index(IrArrayCell *self, IrExpression *index);

#endif /* IR_ARRAY_CELL_INC_X */
