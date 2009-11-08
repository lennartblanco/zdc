#ifndef IR_ARRAY_CELL_REF_INC_X
#define IR_ARRAY_CELL_REF_INC_X

#include "ir_lvalue.h"
#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY_CELL_REF ir_array_cell_ref_get_type()

#define IR_ARRAY_CELL_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY_CELL_REF, IrArrayCellRef))

#define IR_ARRAY_CELL_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY_CELL_REF, IrArrayCellRefClass))

#define IR_IS_ARRAY_CELL_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ARRAY_CELL_REF))

#define IR_IS_ARRAY_CELL_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY_CELL_REF))

#define IR_ARRAY_CELL_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY_CELL_REF, IrArrayCellRefClass))

typedef struct
{
    IrLvalue parent;

    /* private */
    IrExpression *index;
    DtDataType   *data_type;
} IrArrayCellRef;

typedef struct
{
    IrLvalueClass parent_class;
} IrArrayCellRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_cell_ref_get_type(void);

IrArrayCellRef *
ir_array_cell_ref_new(char *array_name,
                      IrExpression *index,
                      guint line_number);

/**
 * get array name
 */
char *
ir_array_cell_get_name(IrArrayCellRef *self);

/**
 * set array symbol
 */
void
ir_array_cell_ref_set_symbol(IrArrayCellRef *self, IrVariable *array_symbol);

/**
 * get array symbol
 */
IrVariable *
ir_array_cell_ref_get_symbol(IrArrayCellRef *self);

/**
 * get array index expression
 */
IrExpression *
ir_array_cell_ref_get_index(IrArrayCellRef *self);

void
ir_array_cell_ref_set_index(IrArrayCellRef *self, IrExpression *index);

#endif /* IR_ARRAY_CELL_REF_INC_X */
