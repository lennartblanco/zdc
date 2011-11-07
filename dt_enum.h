#ifndef DT_ENUM_INC_X
#define DT_ENUM_INC_X

#include "dt_user.h"
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
    DtUser parent;

    /* private */
    DtDataType *base_type;
    GSList *members;
};

typedef struct
{
    DtUserClass parent_class;
} DtEnumClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_enum_get_type(void);

DtEnum *
dt_enum_new(gchar *name,
            DtDataType *base_type,
            IrModule *parent_module,
            guint line_number);

gchar *
dt_enum_get_tag(DtEnum *self);

bool
dt_enum_is_anonymous(DtEnum *self);

void
dt_enum_set_members(DtEnum *self, GSList *members);

/**
 * @return enum members as a list of IrEnumMember objects
 */
GSList *
dt_enum_get_members(DtEnum *self);

IrEnumMember *
dt_enum_get_member(DtEnum *self, const gchar *enum_member_name);

DtDataType *
dt_enum_get_base_type(DtEnum *self);

void
dt_enum_set_base_type(DtEnum *self, DtDataType *base_type);

#endif /* DT_ENUM_INC_X */
