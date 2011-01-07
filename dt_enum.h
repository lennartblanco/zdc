#ifndef DT_ENUM_INC_X
#define DT_ENUM_INC_X

#include "dt_data_type.h"
#include "ir_enum_member.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_ENUM dt_enum_get_type()

#define DT_ENUM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_ENUM, DtEnum))

#define DT_ENUM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_ENUM, DtEnumClass))

#define DT_IS_ENUM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_ENUM))

#define DT_IS_ENUM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_ENUM))

#define DT_ENUM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_ENUM, DtEnumClass))

struct _DtEnum
{
    DtDataType parent;

    /* private */
    gchar *name;
    DtDataType *base_type;
    IrEnumMember *first_member;
    IrModule *parent_module;
    char *mangled_name;
};

typedef struct
{
    DtDataTypeClass parent_class;
} DtEnumClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_enum_get_type(void);

DtEnum *
dt_enum_new(gchar *name,
            DtDataType *base_type,
            IrModule *parent_module);

void
dt_enum_set_first_member(DtEnum *self,
                         IrEnumMember *first_member);

DtDataType *
dt_enum_get_base_type(DtEnum *self);

void
dt_enum_set_base_type(DtEnum *self, DtDataType *base_type);

#endif /* DT_ENUM_INC_X */
