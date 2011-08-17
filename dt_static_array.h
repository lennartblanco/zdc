#ifndef DT_STATIC_ARRAY_INC_X
#define DT_STATIC_ARRAY_INC_X

#include "dt_array.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_STATIC_ARRAY dt_static_array_get_type()

#define DT_STATIC_ARRAY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_STATIC_ARRAY, DtStaticArray))

#define DT_STATIC_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_STATIC_ARRAY, DtStaticArrayClass))

#define DT_IS_STATIC_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_STATIC_ARRAY))

#define DT_IS_STATIC_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_STATIC_ARRAY))

#define DT_STATIC_ARRAY_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_STATIC_ARRAY, DtStaticArrayClass))

#define DT_STATIC_ARRAY_MAX_SIZE ((16 * 1024 * 1024) - 1)

typedef struct
{
    DtArray parent;
    /* private */
    guint32 length;
    char *string_of;
    char *mangled_name;
} DtStaticArray;

typedef struct
{
    DtArrayClass parent_class;
} DtStaticArrayClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_static_array_get_type(void);

DtStaticArray *
dt_static_array_new(DtDataType *data_type, guint32 length, guint line_number);

DtDataType *
dt_static_array_get_data_type(DtStaticArray *self);

guint32
dt_static_array_get_length(DtStaticArray *self);

#endif /* DT_STATIC_ARRAY_INC_X */
