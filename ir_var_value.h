#ifndef IR_VAR_VALUE_INC_X
#define IR_VAR_VALUE_INC_X

/*
 * Variable value used in an expression.
 *
 * The identifiers in the source code are looked up in the symbol table and
 * are replaced with this object. This object records the referenced variable
 * and line number where it is used.
 */

#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_VAR_VALUE ir_var_value_get_type()

#define IR_VAR_VALUE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_VAR_VALUE, IrVarValue))

#define IR_VAR_VALUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_VAR_VALUE, IrVarValueClass))

#define IR_IS_VAR_VALUE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_VAR_VALUE))

#define IR_IS_VAR_VALUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_VAR_VALUE))

#define IR_VAR_VALUE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_VAR_VALUE, IrVarValueClass))

typedef struct
{
    IrExpression parent;

    /* private */
    IrVariable *var;
} IrVarValue;

typedef struct
{
    IrExpressionClass parent_class;
} IrVarValueClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_var_value_get_type(void);

IrVarValue *
ir_var_value_new(IrVariable *var, guint line_number);

IrVariable *
ir_var_value_get_var(IrVarValue *self);

#endif /* IR_VAR_VALUE_INC_X */
