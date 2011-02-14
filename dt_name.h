#ifndef DT_NAME_INC_X
#define DT_NAME_INC_X

/**
 * A place-holder for a user defined type, e.g. alias or enum. When a user
 * defined type is used, only it's name is know when AST is build, thus we
 * need an object to hold the name.
 */

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_NAME dt_name_get_type()

#define DT_NAME(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_NAME, DtName))

#define DT_NAME_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_NAME, DtNameClass))

#define DT_IS_NAME(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_NAME))

#define DT_IS_NAME_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_NAME))

#define DT_NAME_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_NAME, DtNameClass))

typedef struct
{
    DtDataType parent;

    /* private */
    gchar *name;
    guint line_number;
} DtName;

typedef struct
{
    DtDataTypeClass parent_class;
} DtNameClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_name_get_type(void);

DtName *
dt_name_new(gchar *name, guint line_number);

gchar *
dt_name_get_name(DtName *self);

guint
dt_name_get_line_num(DtName *self);

#endif /* DT_NAME_INC_X */
