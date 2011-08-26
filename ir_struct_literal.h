#ifndef IR_STRUCT_LITERAL_INC_X
#define IR_STRUCT_LITERAL_INC_X

#include "ir_literal.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_STRUCT_LITERAL ir_struct_literal_get_type()

#define IR_STRUCT_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_STRUCT_LITERAL, IrStructLiteral))

#define IR_STRUCT_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_STRUCT_LITERAL, IrStructLiteralClass))

#define IR_IS_STRUCT_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_STRUCT_LITERAL))

#define IR_STRUCT_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_STRUCT_LITERAL, IrStructLiteralClass))

typedef struct
{
    IrLiteral parent;

    /* private */
    GSList *members;
} IrStructLiteral;

typedef struct
{
    IrLiteralClass parent_class;
} IrStructLiteralClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_literal_get_type(void);

bool
ir_is_struct_literal(void *obj);

IrStructLiteral *
ir_struct_literal_new(GSList *members);

GSList *
ir_struct_literal_get_members(IrStructLiteral *self);


#endif /* IR_STRUCT_LITERAL_INC_X */
