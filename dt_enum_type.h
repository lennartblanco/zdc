#ifndef DT_ENUM_TYPE_INC_X
#define DT_ENUM_TYPE_INC_X

#include "dt_data_type.h"
#include "ir_enum_member.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_ENUM_TYPE dt_enum_type_get_type()

#define DT_ENUM_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_ENUM_TYPE, DtEnumType))

#define DT_ENUM_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_ENUM_TYPE, DtEnumTypeClass))

#define DT_IS_ENUM_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_ENUM_TYPE))

#define DT_IS_ENUM_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_ENUM_TYPE))

#define DT_ENUM_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_ENUM_TYPE, DtEnumTypeClass))

struct _DtEnumType
{
    DtDataType parent;

    /* private */
    gchar *name;
    DtDataType *base_type;
    IrEnumMember *first_member;
};

typedef struct
{
    DtDataTypeClass parent_class;
} DtEnumTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_enum_type_get_type(void);

DtEnumType *
dt_enum_type_new(gchar *name,
                 DtDataType *base_type);

void
dt_enum_type_set_first_member(DtEnumType *self,
                              IrEnumMember *first_member);

DtDataType *
dt_enum_type_get_base_type(DtEnumType *self);

void
dt_enum_type_set_base_type(DtEnumType *self, DtDataType *base_type);

#endif /* DT_ENUM_TYPE_INC_X */
