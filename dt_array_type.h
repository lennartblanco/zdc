#ifndef DT_ARRAY_TYPE_INC_X
#define DT_ARRAY_TYPE_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_ARRAY_TYPE dt_array_type_get_type()

#define DT_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_ARRAY_TYPE, DtArrayType))

#define DT_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_ARRAY_TYPE, DtArrayTypeClass))

#define DT_IS_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_ARRAY_TYPE))

#define DT_IS_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_ARRAY_TYPE))

#define DT_ARRAY_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_ARRAY_TYPE, DtArrayTypeClass))

typedef struct
{
    DtDataType parent;
    /* private */
    basic_data_type_t data_type;
} DtArrayType;

typedef struct
{
    DtDataTypeClass parent_class;
} DtArrayTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_array_type_get_type(void);

DtArrayType *
dt_array_type_new(basic_data_type_t data_type);

basic_data_type_t
dt_array_type_get_data_type(DtArrayType *self);

guint32
dt_array_type_get_length(DtArrayType *self);

#endif /* DT_ARRAY_TYPE_INC_X */
