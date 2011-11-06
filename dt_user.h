#ifndef DT_USER_INC_X
#define DT_USER_INC_X

/**
 * Abstract parent class for user defined data types.
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
    char *mangled_name;
} DtUser;

typedef struct
{
    DtDataTypeClass parent_class;

    /* public virtual methods */
    gchar * (*get_mangled_prefix) (DtUser *self);
} DtUserClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_user_get_type(void);

gchar *
dt_user_get_mangled_prefix(DtUser *self);

#endif /* DT_USER_INC_X */
