#include "dt_data_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_data_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtDataTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtDataType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "DtDataTypeType",
                                    &info, 0);
    }
    return type;
}

void
dt_data_type_print(DtDataType *self, FILE *out)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(out);

    DT_DATA_TYPE_GET_CLASS(self)->do_print(self, out);
}

