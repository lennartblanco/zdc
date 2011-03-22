#ifndef IR_PTR_DREF_INC_X
#define IR_PTR_DREF_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_PTR_DREF ir_ptr_dref_get_type()

#define IR_PTR_DREF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_PTR_DREF, IrPtrDref))

#define IR_PTR_DREF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_PTR_DREF, IrPtrDrefClass))

#define IR_IS_PTR_DREF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_PTR_DREF))

#define IR_IS_PTR_DREF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_PTR_DREF))

#define IR_PTR_DREF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_PTR_DREF, IrPtrDrefClass))

typedef struct
{
    IrExpression parent;
    /* private */
    IrExpression *ptr_expression;
} IrPtrDref;

typedef struct
{
    IrExpressionClass parent_class;
} IrPtrDrefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ptr_dref_get_type(void);

IrPtrDref *
ir_ptr_dref_new(IrExpression *ptr_expression, guint line_number);

IrExpression *
ir_ptr_dref_get_expression(IrPtrDref *self);

void
ir_ptr_dref_set_expression(IrPtrDref *self, IrExpression *ptr_expression);

/**
 * Returns storage size, in bytes, of the dereferenced memory destination.
 */
guint
ir_ptr_dref_get_dest_size(IrPtrDref *self);

#endif /* IR_PTR_DREF_INC_X */
