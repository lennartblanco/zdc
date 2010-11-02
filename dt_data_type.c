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
dt_data_type_set_immutable(DtDataType *self, bool is_immutable)
{
    assert(DT_IS_DATA_TYPE(self));

    self->immutable = true;
}

bool
dt_data_type_is_immutalbe(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    return self->immutable;
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

    char *str = DT_DATA_TYPE_GET_CLASS(self)->get_string(self);

    if (self->immutable)
    {
        char *tmp = g_strdup_printf("immutable(%s)", str);
        str = tmp;
    }

    return str;
}

char *
dt_data_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_DATA_TYPE_GET_CLASS(self)->get_mangled != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->get_mangled(self);
}

IrExpression *
dt_data_type_get_init(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_DATA_TYPE_GET_CLASS(self)->get_init != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->get_init(self);
}

bool
dt_data_type_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_IS_DATA_TYPE(type));
    assert(DT_DATA_TYPE_GET_CLASS(self)->is_same != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->is_same(self, type);
}

