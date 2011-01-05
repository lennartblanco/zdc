#ifndef IR_ASSIGNMENT_INC_X
#define IR_ASSIGNMENT_INC_X

#include "ir_statment.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ASSIGNMENT ir_assignment_get_type()

#define IR_ASSIGNMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ASSIGNMENT, IrAssignment))

#define IR_ASSIGNMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ASSIGNMENT, IrAssignmentClass))

#define IR_IS_ASSIGNMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ASSIGNMENT))

#define IR_IS_ASSIGNMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ASSIGNMENT))

#define IR_ASSIGNMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ASSIGNMENT, IrAssignmentClass))

typedef struct
{
    IrStatment          parent;

    /* private */
    IrExpression       *lvalue;
    IrExpression       *value;
} IrAssignment;

typedef struct
{
    IrStatmentClass parent_class;
} IrAssignmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_assignment_get_type(void);

IrAssignment *
ir_assignment_new(IrExpression *lvalue,
                  IrExpression *value,
                  guint line_number);

IrExpression *
ir_assignment_get_lvalue(IrAssignment *self);

void
ir_assignment_set_lvalue(IrAssignment *self, IrExpression *lvalue);

IrExpression *
ir_assignment_get_value(IrAssignment *self);

void
ir_assignment_set_value(IrAssignment *self, IrExpression *value);

#endif /* IR_ASSIGNMENT_INC_X */
