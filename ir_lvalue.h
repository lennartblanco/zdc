#ifndef IR_LVALUE_INC_X
#define IR_LVALUE_INC_X

/**
 * An assignable expression. An abstract class.
 */

#include "ir_expression.h"
#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_LVALUE ir_lvalue_get_type()

#define IR_LVALUE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_LVALUE, IrLvalue))

#define IR_LVALUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_LVALUE, IrLvalueClass))

#define IR_IS_LVALUE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_LVALUE))

#define IR_IS_LVALUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_LVALUE))

#define IR_LVALUE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_LVALUE, IrLvalueClass))

typedef struct
{
    IrExpression parent;

    /* private */
    char *symbol_name;
    IrVariable *variable;
} IrLvalue;

typedef struct
{
    IrExpressionClass parent_class;
} IrLvalueClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_lvalue_get_type(void);

char *
ir_lvalue_get_name(IrLvalue *self);

void
ir_lvalue_set_variable(IrLvalue *self,
                       IrVariable *variable);

IrVariable *
ir_lvalue_get_variable(IrLvalue *self);

#endif /* IR_LVALUE_INC_X */
