#ifndef DT_AUTO_TYPE_INC_X
#define DT_AUTO_TYPE_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_AUTO_TYPE dt_auto_type_get_type()

#define DT_AUTO_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_AUTO_TYPE, DtAutoType))

#define DT_AUTO_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_AUTO_TYPE, DtAutoTypeClass))

#define DT_IS_AUTO_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_AUTO_TYPE))

#define DT_IS_AUTO_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_AUTO_TYPE))

#define DT_AUTO_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_AUTO_TYPE, AstAutoTypeClass))

typedef struct
{
    DtDataType parent;
} DtAutoType;

typedef struct
{
    DtDataTypeClass parent_class;
} DtAutoTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_auto_type_get_type(void);

DtAutoType * 
dt_auto_type_new();

#endif /* DT_AUTO_TYPE_INC_X */
