#include "dt_alias.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_alias_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtAliasClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtAlias),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_USER,
                                    "DtAliasType",
                                    &info, 0);
    }
    return type;
}

DtAlias *
dt_alias_new(DtDataType *aliased_type,
             gchar *name)
{
    assert(DT_IS_DATA_TYPE(aliased_type));

    DtAlias *obj;

    obj = g_object_new(DT_TYPE_ALIAS,
                       "ir-symbol-name", name,
                       NULL);

    obj->aliased_type = aliased_type;

    return obj;
}

const char *
dt_alias_get_name(DtAlias *self)
{
    assert(DT_IS_ALIAS(self));

    return ir_symbol_get_name(ir_symbol(self));
}

DtDataType *
dt_alias_get_aliased_type(DtAlias *self)
{
    assert(DT_IS_ALIAS(self));

    return self->aliased_type;
}
