#ifndef DT_CLASS_INC_X
#define DT_CLASS_INC_X

#include "dt_record.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_CLASS dt_class_get_type()

#define DT_CLASS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_CLASS, DtClass))

#define DT_CLASS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_CLASS, DtClassClass))

#define DT_IS_CLASS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_CLASS))

#define DT_IS_CLASS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_CLASS))

#define DT_CLASS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_CLASS, DtClassClass))

typedef struct
{
    DtRecord parent;
    /* private */
    IrStructLiteral *blob_init;
} DtClass;

typedef struct
{
    DtRecordClass parent_class;
} DtClassClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_class_get_type(void);

DtClass *
dt_class_new(gchar *name, GSList *members, IrModule *parent_module);

#endif /* DT_CLASS_INC_X */
