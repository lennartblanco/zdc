#ifndef IR_ASSIGMENT_INC_X
#define IR_ASSIGMENT_INC_X

#include "ast_variable_ref.h"
#include "ir_statment.h"
#include "ir_expression.h"
#include "ir_lvalue.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ASSIGMENT ir_assigment_get_type()

#define IR_ASSIGMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ASSIGMENT, IrAssigment))

#define IR_ASSIGMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ASSIGMENT, IrAssigmentClass))

#define IR_IS_ASSIGMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ASSIGMENT))

#define IR_IS_ASSIGMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ASSIGMENT))

#define IR_ASSIGMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ASSIGMENT, IrAssigmentClass))

typedef struct
{
    IrStatment          parent;

    /* private */
    IrLvalue           *lvalue;
    IrExpression       *value;
} IrAssigment;

typedef struct
{
    IrStatmentClass parent_class;
} IrAssigmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_assigment_get_type(void);

IrAssigment *
ir_assigment_new(IrLvalue *lvalue,
                 IrExpression *value,
                 guint line_number);

IrLvalue *
ir_assigment_get_lvalue(IrAssigment *self);

void
ir_assigment_set_lvalue(IrAssigment *self, IrLvalue *lvalue);

IrExpression *
ir_assigment_get_value(IrAssigment *self);

void
ir_assigment_set_value(IrAssigment *self, IrExpression *value);

#endif /* IR_ASSIGMENT_INC_X */
