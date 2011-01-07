#ifndef DT_AUTO_INC_X
#define DT_AUTO_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_AUTO dt_auto_get_type()

#define DT_AUTO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_AUTO, DtAuto))

#define DT_AUTO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_AUTO, DtAutoClass))

#define DT_IS_AUTO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_AUTO))

#define DT_IS_AUTO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_AUTO))

#define DT_AUTO_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_AUTO, AstAutoClass))

typedef struct
{
    DtDataType parent;
} DtAuto;

typedef struct
{
    DtDataTypeClass parent_class;
} DtAutoClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_auto_get_type(void);

DtAuto *
dt_auto_new();

#endif /* DT_AUTO_INC_X */
