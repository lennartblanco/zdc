#ifndef IR_EXPRESSION_INC_X
#define IR_EXPRESSION_INC_X

#include <glib-object.h>

#include "ir_statment.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_EXPRESSION ir_expression_get_type()

#define IR_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_EXPRESSION, IrExpression))

#define IR_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_EXPRESSION, IrExpressionClass))

#define IR_IS_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_EXPRESSION))

#define IR_IS_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_EXPRESSION))

#define IR_EXPRESSION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_EXPRESSION, IrExpressionClass))

typedef struct 
{
  IrStatment parent;
} IrExpression;

typedef struct 
{
  IrStatmentClass parent_class;
  /* public virtual methods */
  DtDataType * (*do_get_data_type) (IrExpression *self);
} IrExpressionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_expression_get_type(void);

DtDataType *
ir_expression_get_data_type(IrExpression *self);

#endif /* IR_EXPRESSION_INC_X */
