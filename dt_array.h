#ifndef DT_ARRAY_INC_X
#define DT_ARRAY_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_ARRAY dt_array_get_type()

#define DT_ARRAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_ARRAY, DtArrayClass))

#define DT_IS_ARRAY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_ARRAY))

#define DT_IS_ARRAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_ARRAY))

#define DT_ARRAY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_ARRAY, DtArrayClass))

typedef struct
{
    DtDataType parent;
    /* private */
    DtDataType *data_type; /* @todo: rename to element_type ? */
    char *string_of;
    char *mangled_name;
} DtArray;

typedef struct
{
    DtDataTypeClass parent_class;
} DtArrayClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_array_get_type(void);

DtArray *
dt_array(void *obj);

DtArray *
dt_array_new(DtDataType *data_type);

guint32
dt_array_get_length(DtArray *self);

/**
 * Get element data type
 */
DtDataType *
dt_array_get_element_type(DtArray *self);

/**
 * @return element's storage size in bytes
 */
guint
dt_array_get_element_size(DtArray *self);

bool
dt_array_is_impl_conv(DtDataType *self, IrExpression *expression);

#endif /* DT_ARRAY_INC_X */
