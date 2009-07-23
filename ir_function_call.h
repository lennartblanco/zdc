#ifndef IR_FUNCTION_CALL_INC_X
#define IR_FUNCTION_CALL_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FUNCTION_CALL ir_function_call_get_type()

#define IR_FUNCTION_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FUNCTION_CALL, IrFunctionCall))

#define IR_FUNCTION_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FUNCTION_CALL, IrFunctionCallClass))

#define IR_IS_FUNCTION_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FUNCTION_CALL))

#define IR_IS_FUNCTION_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FUNCTION_CALL))

#define IR_FUNCTION_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FUNCTION_CALL, IrFunctionCallClass))

typedef struct
{
    IrExpression parent;
    /* private */
    char                 *name;
    GSList               *arguments;
    AstDataType          *return_type;
} IrFunctionCall;

typedef struct
{
    IrExpressionClass parent_class;
} IrFunctionCallClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_function_call_get_type(void);

/**
 *
 * @param arguments arguments for the call, as a list of IrExpression objects
 *                  in _reversed_ order
 */
IrFunctionCall *
ir_function_call_new(AstDataType *return_type,
                     char *name,
                     GSList *arguments);

GSList *
ir_function_call_get_arguments(IrFunctionCall *self);

char *
ir_function_call_get_name(IrFunctionCall *self);

#endif /* IR_FUNCTION_CALL_INC_X */
