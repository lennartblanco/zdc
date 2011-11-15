#ifndef IR_VAR_REF_INC_X
#define IR_VAR_REF_INC_X

/*
 * Variable reference used in an expression.
 *
 * The identifiers in the source code are looked up in the symbol table. If the
 * looked up symbol is a reference variable the ident object is replaced by
 * this object. This object records the referenced variable and line number
 * where it is used.
 */

#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_VAR_REF ir_var_ref_get_type()

#define IR_VAR_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_VAR_REF, IrVarRef))

#define IR_VAR_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_VAR_REF, IrVarRefClass))

#define IR_IS_VAR_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_VAR_REF))

#define IR_IS_VAR_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_VAR_REF))

#define IR_VAR_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_VAR_REF, IrVarRefClass))

typedef struct
{
    IrExpression parent;

    /* private */
    IrVariable *var;
} IrVarRef;

typedef struct
{
    IrExpressionClass parent_class;
} IrVarRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_var_ref_get_type(void);

IrVarRef *
ir_var_ref_new(IrVariable *var, guint line_number);

IrVariable *
ir_var_ref_get_var(IrVarRef *self);

#endif /* IR_VAR_REF_INC_X */
