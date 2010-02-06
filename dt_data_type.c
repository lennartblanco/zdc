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

guint
dt_data_type_get_size(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    return DT_DATA_TYPE_GET_CLASS(self)->get_size(self);
}

/**
 * @return the source string representation of this data type.
 */
char *
dt_data_type_get_string(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    return DT_DATA_TYPE_GET_CLASS(self)->get_string(self);
}

char *
dt_data_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    return DT_DATA_TYPE_GET_CLASS(self)->get_mangled(self);
}

