#ifndef DT_VOID_INC_X
#define DT_VOID_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_VOID dt_void_get_type()

#define DT_VOID(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_VOID, DtVoid))

#define DT_VOID_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_VOID, DtVoidClass))

#define DT_IS_VOID(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_VOID))

#define DT_IS_VOID_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_VOID))

#define DT_VOID_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_VOID, AstVoidClass))

typedef struct
{
    DtDataType parent;
} DtVoid;

typedef struct
{
    DtDataTypeClass parent_class;
} DtVoidClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_void_get_type(void);

DtVoid *
dt_void_new();

#endif /* DT_VOID_INC_X */
