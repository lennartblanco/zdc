#ifndef IR_IDENT_INC_X
#define IR_IDENT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_IDENT ir_ident_get_type()

#define IR_IDENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_IDENT, IrIdent))

#define IR_IDENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_IDENT, IrIdentClass))

#define IR_IS_IDENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_IDENT))

#define IR_IS_IDENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_IDENT))

#define IR_IDENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_IDENT, IrIdentClass))

typedef struct
{
  IrExpression parent;

  /* private */
  gchar *name;
} IrIdent;

typedef struct
{
  IrExpressionClass parent_class;
} IrIdentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ident_get_type(void);

IrIdent *
ir_ident_new(const gchar *name);

const gchar *
ir_ident_get_name(IrIdent *self);

#endif /* IR_IDENT_INC_X */
