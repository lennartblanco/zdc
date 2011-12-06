#ifndef IR_METHOD_CALL_INC_X
#define IR_METHOD_CALL_INC_X

#include "ir_call.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_METHOD_CALL ir_method_call_get_type()

#define IR_METHOD_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_METHOD_CALL, IrMethodCall))

#define IR_METHOD_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_METHOD_CALL, IrMethodCallClass))

#define IR_IS_METHOD_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_METHOD_CALL))

#define IR_IS_METHOD_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_METHOD_CALL))

#define IR_METHOD_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_METHOD_CALL, IrMethodCallClass))

typedef struct
{
    IrCall parent;

    /* private */
    IrExpression *this_exp;
    const char *method_name;
} IrMethodCall;

typedef struct
{
    IrCallClass parent_class;
} IrMethodCallClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_method_call_get_type(void);

IrMethodCall *
ir_method_call_new(IrExpression *this_exp,
                   const char *method_name,
                   GSList *arguments,
                   guint line_number);

void
ir_method_call_set_this_exp(IrMethodCall *self, IrExpression *this_exp);

IrExpression *
ir_method_call_get_this_exp(IrMethodCall *self);

/**
 * @return called method's name
 */
const char *
ir_method_call_get_name(IrMethodCall *self);

#endif /* IR_METHOD_CALL_INC_X */
