#ifndef DT_USER_INC_X
#define DT_USER_INC_X

/**
 * A place-holder for a user defined type, e.g. alias or enum. When a user
 * defined type is used, only it's name is know when AST is build, thus we
 * need an object to hold the name.
 */

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_USER dt_user_get_type()

#define DT_USER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_USER, DtUser))

#define DT_USER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_USER, DtUserClass))

#define DT_IS_USER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_USER))

#define DT_IS_USER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_USER))

#define DT_USER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_USER, DtUserClass))

typedef struct
{
    DtDataType parent;

    /* private */
    gchar *name;
    guint line_number;
} DtUser;

typedef struct
{
    DtDataTypeClass parent_class;
} DtUserClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_user_get_type(void);

DtUser *
dt_user_new(gchar *name, guint line_number);

gchar *
dt_user_get_name(DtUser *self);

guint
dt_user_get_line_num(DtUser *self);

#endif /* DT_USER_INC_X */
