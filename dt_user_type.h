#ifndef DT_USER_TYPE_INC_X
#define DT_USER_TYPE_INC_X

/**
 * A placeholder for a user defined type, e.g. alias or enum. When a user
 * defined type is used, only it's name is know when AST is build, thus we
 * need an object to hold the name.
 */

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_USER_TYPE dt_user_type_get_type()

#define DT_USER_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_USER_TYPE, DtUserType))

#define DT_USER_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_USER_TYPE, DtUserTypeClass))

#define DT_IS_USER_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_USER_TYPE))

#define DT_IS_USER_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_USER_TYPE))

#define DT_USER_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_USER_TYPE, DtUserTypeClass))

typedef struct
{
    DtDataType parent;

    /* private */
    gchar *name;
    guint line_number;
} DtUserType;

typedef struct
{
    DtDataTypeClass parent_class;
} DtUserTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_user_type_get_type(void);

DtUserType *
dt_user_type_new(gchar *name, guint line_number);

gchar *
dt_user_type_get_name(DtUserType *self);

guint
dt_user_type_get_line_num(DtUserType *self);

#endif /* DT_USER_TYPE_INC_X */
