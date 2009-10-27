#ifndef DT_STATIC_ARRAY_TYPE_INC_X
#define DT_STATIC_ARRAY_TYPE_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_STATIC_ARRAY_TYPE dt_static_array_type_get_type()

#define DT_STATIC_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_STATIC_ARRAY_TYPE, DtStaticArrayType))

#define DT_STATIC_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_STATIC_ARRAY_TYPE, DtStaticArrayTypeClass))

#define DT_IS_STATIC_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_STATIC_ARRAY_TYPE))

#define DT_IS_STATIC_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_STATIC_ARRAY_TYPE))

#define DT_STATIC_ARRAY_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_STATIC_ARRAY_TYPE, DtStaticArrayTypeClass))

typedef struct
{
    DtDataType parent;
    /* private */
    basic_data_type_t data_type;
    guint32 length;
} DtStaticArrayType;

typedef struct
{
    DtDataTypeClass parent_class;
} DtStaticArrayTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_static_array_type_get_type(void);

DtStaticArrayType *
dt_static_array_type_new(basic_data_type_t data_type, guint32 length);

basic_data_type_t
dt_static_array_type_get_data_type(DtStaticArrayType *self);

guint32
dt_static_array_type_get_length(DtStaticArrayType *self);

#endif /* DT_STATIC_ARRAY_TYPE_INC_X */
