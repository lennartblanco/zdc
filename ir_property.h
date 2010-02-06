#ifndef IR_PROPERTY_INC_X
#define IR_PROPERTY_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_PROPERTY ir_property_get_type()

#define IR_PROPERTY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_PROPERTY, IrProperty))

#define IR_PROPERTY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_PROPERTY, IrPropertyClass))

#define IR_IS_PROPERTY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_PROPERTY))

#define IR_IS_PROPERTY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_PROPERTY))

#define IR_PROPERTY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_PROPERTY, IrPropertyClass))

#define IR_PROP_INIT   "init"
#define IR_PROP_SIZEOF "sizeof"
#define IR_PROP_LENGTH "length"

typedef enum ir_property_id_e
{
  ir_prop_init,
  ir_prop_sizeof,
  ir_prop_length
} ir_property_id_t;

typedef struct
{
    IrExpression parent;

    /* private */
    IrExpression *exp;
    ir_property_id_t id;
} IrProperty;

typedef struct
{
    IrExpressionClass parent_class;
} IrPropertyClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_property_get_type(void);

/**
 * @return NULL if specified property name is illegal
 */
IrProperty *
ir_property_new(IrExpression *exp, const char *name, guint line_number);

void
ir_property_set_expression(IrProperty *self, IrExpression *exp);

IrExpression *
ir_property_get_expression(IrProperty *self);

ir_property_id_t
ir_property_get_id(IrProperty *self);

#endif /* IR_PROPERTY_INC_X */
