#ifndef IR_LITERAL_INC_X
#define IR_LITERAL_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_LITERAL ir_literal_get_type()

#define IR_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_LITERAL, IrLiteralClass))

#define IR_IS_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_LITERAL))

#define IR_IS_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_LITERAL))

#define IR_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_LITERAL, IrLiteralClass))

typedef struct
{
    IrExpression parent;

    /* private */
    char *data_label;
} IrLiteral;

typedef struct
{
  IrExpressionClass parent_class;
} IrLiteralClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_literal_get_type(void);

IrLiteral *
ir_literal(void *obj);

/**
 * assign a label in .data section for this literal
 */
void
ir_literal_set_data_label(IrLiteral *self, char *label);

/**
 * @return this literal's label in .data section, or NULL if
 *         no label have been assigned
 */
char *
ir_literal_get_data_label(IrLiteral *self);

#endif /* IR_LITERAL_INC_X */
