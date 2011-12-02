#ifndef IR_CAST_INC_X
#define IR_CAST_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_CAST ir_cast_get_type()

#define IR_CAST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_CAST, IrCastClass))

#define IR_IS_CAST(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_CAST))

#define IR_IS_CAST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_CAST))

#define IR_CAST_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_CAST, IrCastClass))

typedef struct
{
    IrExpression parent;
    /* private */
    DtDataType *target_type;
    IrExpression *value;
} IrCast;

typedef struct
{
    IrExpressionClass parent_class;
} IrCastClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_cast_get_type(void);

IrCast *
ir_cast(void *obj);

/**
 * @param target_type the type to convert the value to
 * @param value the expression to type convert
 */
IrCast *
ir_cast_new(DtDataType *target_type, IrExpression *value, guint line_number);

DtDataType *
ir_cast_get_target_type(IrCast *self);

void
ir_cast_set_value(IrCast *self, IrExpression *value);

IrExpression *
ir_cast_get_value(IrCast *self);

#endif /* IR_CAST_INC_X */
