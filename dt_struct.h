#ifndef DT_STRUCT_INC_X
#define DT_STRUCT_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_STRUCT dt_struct_get_type()

#define DT_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_STRUCT, DtStruct))

#define DT_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_STRUCT, DtStructClass))

#define DT_IS_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_STRUCT))

#define DT_IS_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_STRUCT))

#define DT_STRUCT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_STRUCT, DtStructClass))

typedef struct
{
    DtDataType parent;

    /* private */
    guint size;
} DtStruct;

typedef struct
{
    DtDataTypeClass parent_class;
} DtStructClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_struct_get_type(void);

DtStruct *
dt_struct_new(GSList *member_types);

#endif /* DT_STRUCT_INC_X */
