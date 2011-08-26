#ifndef IR_ARRAY_LITERAL_INC_X
#define IR_ARRAY_LITERAL_INC_X

#include "dt_array.h"
#include "ir_literal.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY_LITERAL ir_array_literal_get_type()

#define IR_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY_LITERAL, IrArrayLiteral))

#define IR_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY_LITERAL, IrArrayLiteralClass))

#define IR_IS_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY_LITERAL))

#define IR_ARRAY_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY_LITERAL, IrArrayLiteralClass))

typedef struct
{
    IrLiteral parent;

    /* private */
    DtArray *data_type;
    GSList *values;
    char *data_label;
} IrArrayLiteral;

typedef struct
{
    IrLiteralClass parent_class;
} IrArrayLiteralClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_literal_get_type(void);

bool
ir_is_array_literal(void *obj);

IrArrayLiteral *
ir_array_literal_new(guint line_number);

void
ir_array_literal_add_value(IrArrayLiteral *self, IrExpression *value);

void
ir_array_literal_set_values(IrArrayLiteral *self, GSList *values);

GSList *
ir_array_literal_get_values(IrArrayLiteral *self);

/**
 * get number of elements in this array literal expression
 */
guint
ir_array_literal_get_length(IrArrayLiteral *self);

/**
 * @return get storage size in bytes for this array literal
 */
guint
ir_array_literal_get_size(IrArrayLiteral *self);

#endif /* IR_ARRAY_LITERAL_INC_X */
