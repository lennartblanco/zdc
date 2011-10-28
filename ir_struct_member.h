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
    IrExpression *base;
    DtDataType *type;
    guint offset;
    guint padding;
    IrExpression *init;
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

/**
 * @param offset the members offset in the struct
 * @param padding number of padding bytes
 * @param init the default initialization expression for this member
 */
IrStructMember *
ir_struct_member_new(DtDataType *type,
                     guint offset,
                     guint padding,
                     IrExpression *init);

/**
 * Set the base expression for this struct member. Base expression denotes the
 * struct instance where we want to access the member.
 *
 * E.g.
 *
 * struct some { int a, int b }
 *
 * some foo;
 *
 * foo.bar = 2;
 *
 * 'foo' is the base expression of the foo.bar.
 */
void
ir_struct_member_set_base(IrStructMember *self, IrExpression *base);

IrExpression *
ir_struct_member_get_base(IrStructMember *self);

guint
ir_struct_member_get_offset(IrStructMember *self);

/**
 * Get number of padding bytes the must be inserted after this
 * member.
 */
guint
ir_struct_member_get_padding(IrStructMember *self);

IrExpression *
ir_struct_member_get_init(IrStructMember *self);

#endif /* IR_STRUCT_MEMBER_INC_X */
