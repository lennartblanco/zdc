#ifndef IR_NEW_INC_X
#define IR_NEW_INC_X

#include "ir_expression.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_NEW ir_new_get_type()

#define IR_NEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_NEW, IrNew))

#define IR_NEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_NEW, IrNewClass))

#define IR_IS_NEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_NEW))

#define IR_IS_NEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_NEW))

#define IR_NEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_NEW, IrNewClass))

typedef struct
{
    IrExpression parent;

    /* private */
    DtDataType *type;
    DtDataType *exp_type; /* this new expressions resulting type */
} IrNew;

typedef struct
{
    IrExpressionClass parent_class;
} IrNewClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_new_get_type(void);

IrNew *
ir_new_new(DtDataType *type, guint line_number);

DtDataType *
ir_new_get_dt_type(IrNew *self);

void
ir_new_set_dt_type(IrNew *self, DtDataType *type);

#endif /* IR_NEW_INC_X */
