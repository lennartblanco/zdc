#ifndef IR_ARRAY_LITERAL_INC_X
#define IR_ARRAY_LITERAL_INC_X

#include "dt_static_array_type.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY_LITERAL ir_array_literal_get_type()

#define IR_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY_LITERAL, IrArrayLiteral))

#define IR_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY_LITERAL, IrArrayLiteralClass))

#define IR_IS_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ARRAY_LITERAL))

#define IR_IS_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY_LITERAL))

#define IR_ARRAY_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY_LITERAL, IrArrayLiteralClass))

typedef struct
{
    IrExpression parent;

    /* private */
    DtStaticArrayType *data_type;
    GSList *values;
} IrArrayLiteral;

typedef struct
{
    IrExpressionClass parent_class;
} IrArrayLiteralClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_literal_get_type(void);

IrArrayLiteral *
ir_array_literal_new(guint line_number);

void
ir_array_literal_add_value(IrArrayLiteral *self, IrExpression *value);

void
ir_array_literal_set_values(IrArrayLiteral *self, GSList *values);

GSList *
ir_array_literal_get_values(IrArrayLiteral *self);

#endif /* IR_ARRAY_LITERAL_INC_X */
