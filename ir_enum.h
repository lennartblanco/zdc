#ifndef IR_ENUM_INC_X
#define IR_ENUM_INC_X

#include "ir_symbol.h"
#include "dt_types.h"
#include "dt_enum_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ENUM ir_enum_get_type()

#define IR_ENUM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ENUM, IrEnum))

#define IR_ENUM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ENUM, IrEnumClass))

#define IR_IS_ENUM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ENUM))

#define IR_IS_ENUM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ENUM))

#define IR_ENUM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ENUM, IrEnumClass))

struct _IrEnum
{
    IrSymbol parent;

    /* private */
    DtEnumType *data_type;
    GSList *members;
};

typedef struct 
{
  IrSymbolClass parent_class;
} IrEnumClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_enum_get_type(void);

IrEnum *
ir_enum_new(gchar *tag,
            DtDataType *base_type,
            guint line_number);

gchar *
ir_enum_get_tag(IrEnum *self);

DtDataType *
ir_enum_get_base_type(IrEnum *self);

void
ir_enum_set_base_type(IrEnum *self, DtDataType *base_type);

void
ir_enum_set_members(IrEnum *self, GSList *members);

/**
 * @return enum members as a list of IrEnumMember objects
 */
GSList *
ir_enum_get_members(IrEnum *self);

IrEnumMember *
ir_enum_get_member(IrEnum *self, const gchar *enum_member_name);

DtEnumType *
ir_enum_get_data_type(IrEnum *self);

#endif /* IR_ENUM_INC_X */
