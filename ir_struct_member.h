#ifndef IR_STRUCT_MEMBER_INC_X
#define IR_STRUCT_MEMBER_INC_X

	#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_STRUCT_MEMBER ir_struct_member_get_type()

#define IR_STRUCT_MEMBER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_STRUCT_MEMBER, IrStructMember))

#define IR_STRUCT_MEMBER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_STRUCT_MEMBER, IrStructMemberClass))

#define IR_IS_STRUCT_MEMBER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_STRUCT_MEMBER))

#define IR_IS_STRUCT_MEMBER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_STRUCT_MEMBER))

#define IR_STRUCT_MEMBER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_STRUCT_MEMBER, IStructMemberClass))

typedef struct
{
    IrExpression parent;

    /* private */
    DtDataType *type;
    guint offset;
} IrStructMember;

typedef struct
{
    IrExpressionClass parent_class;
} IrStructMemberClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_member_get_type(void);

IrStructMember *
ir_struct_member_new(DtDataType *type, guint offset);

#endif /* IR_STRUCT_MEMBER_INC_X */
