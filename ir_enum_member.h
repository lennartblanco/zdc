#ifndef IR_ENUM_MEMBER_INC_X
#define IR_ENUM_MEMBER_INC_X

#include "ir_types.h"
#include "dt_enum.h"
#include "ir_symbol.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ENUM_MEMBER ir_enum_member_get_type()

#define IR_ENUM_MEMBER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ENUM_MEMBER, IrEnumMemberClass))

#define IR_IS_ENUM_MEMBER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ENUM_MEMBER))

#define IR_IS_ENUM_MEMBER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ENUM_MEMBER))

#define IR_ENUM_MEMBER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ENUM_MEMBER, IrEnumMemberClass))

struct _IrEnumMember
{
    IrSymbol       parent;

    /*
     * private 
     */
    IrExpression  *value;
    DtEnum *enum_type;
};

typedef struct
{
  IrSymbolClass parent_class;
} IrEnumMemberClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_enum_member_get_type(void);

IrEnumMember *
ir_enum_member(void *obj);

IrEnumMember *
ir_enum_member_new(DtEnum *enum_type, gchar *name, IrExpression *value);

IrExpression *
ir_enum_member_get_value(IrEnumMember *self);

void
ir_enum_member_set_value(IrEnumMember *self, IrExpression *value);

#endif /* IR_ENUM_MEMBER_INC_X */
