#ifndef IR_ARRAY_INC_X
#define IR_ARRAY_INC_X

/**
 * A reference to an array handle.
 *
 * E.g.
 *  int[] x;
 *
 *  x = [1, 2, 3];
 *
 * The lvalue in the assigment is repesented as IrArray object.
 * 
 */

#include "ir_lvalue.h"
#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY ir_array_get_type()

#define IR_ARRAY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY, IrArray))

#define IR_ARRAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY, IrArrayClass))

#define IR_IS_ARRAY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ARRAY))

#define IR_IS_ARRAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY))

#define IR_ARRAY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY, IrArrayClass))

typedef struct
{
    IrLvalue parent;
} IrArray;

typedef struct
{
    IrLvalueClass parent_class;
} IrArrayClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_get_type(void);

IrArray *
ir_array_new(IrVariable *variable);

#endif /* IR_ARRAY_INC_X */
