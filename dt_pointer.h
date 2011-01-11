#ifndef DT_POINTER_INC_X
#define DT_POINTER_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_POINTER dt_pointer_get_type()

#define DT_POINTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_POINTER, DtPointer))

#define DT_POINTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_POINTER, DtPointerClass))

#define DT_IS_POINTER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_POINTER))

#define DT_IS_POINTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_POINTER))

#define DT_POINTER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_POINTER, DtPointerClass))

typedef struct
{
    DtDataType parent;
    /* private */
    DtDataType *base_type;
    char *string_of;
    char *mangled_name;
} DtPointer;

typedef struct
{
    DtDataTypeClass parent_class;
} DtPointerClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_pointer_get_type(void);

DtPointer *
dt_pointer_new(DtDataType *base_type);

DtDataType *
dt_pointer_get_base_type(DtPointer *self);

#endif /* DT_POINTER_INC_X */
