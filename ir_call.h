/**
 * Abstarct base class for function/method calls.
 */
#ifndef IR_CALL_INC_X
#define IR_CALL_INC_X

#include "ir_expression.h"
#include "ir_function.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_CALL ir_call_get_type()

#define IR_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_CALL, IrCall))

#define IR_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_CALL, IrCallClass))

#define IR_IS_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_CALL))

#define IR_IS_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_CALL))

#define IR_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_CALL, IrCallClass))

typedef struct
{
    IrExpression parent;
    /* private */
    IrFunction           *function;
    GSList               *arguments;
} IrCall;

typedef struct
{
    IrExpressionClass parent_class;
    /* public virtual methods */
    IrExpression * (*do_get_this_arg) (IrCall *self);
} IrCallClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_call_get_type(void);

GSList *
ir_call_get_arguments(IrCall *self);

IrExpression *
ir_call_get_this_arg(IrCall *self);

void
ir_call_set_arguments(IrCall *self, GSList *arguments);

void
ir_call_set_function(IrCall *self, IrFunction *function);

IrFunction *
ir_call_get_function(IrCall *self);

ir_linkage_type_t
ir_call_get_linkage(IrCall *self);


#endif /* IR_CALL_INC_X */
