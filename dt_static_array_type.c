#include <stdbool.h>

#include "dt_static_array_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_static_array_type_do_print(DtDataType *self, FILE *out);

static void
dt_static_array_type_class_init(gpointer klass, gpointer dummy);

static char *
dt_static_array_type_get_mangled(DtDataType *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_static_array_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtStaticArrayTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_static_array_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtStaticArrayType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_ARRAY_TYPE,
                                    "DtStaticArrayTypeType",
                                    &info, 0);
    }
    return type;
}

DtStaticArrayType *
dt_static_array_type_new(DtDataType *data_type, guint32 length)
{
    DtStaticArrayType *obj;

    obj = g_object_new(DT_TYPE_STATIC_ARRAY_TYPE, NULL);
    DT_ARRAY_TYPE(obj)->data_type = data_type;
    obj->length = length;
    obj->mangled_name = NULL;

    return obj;
}

DtDataType *
dt_static_array_type_get_data_type(DtStaticArrayType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return dt_array_type_get_data_type(DT_ARRAY_TYPE(self));
}

guint32
dt_static_array_type_get_length(DtStaticArrayType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return self->length;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
dt_static_array_type_do_print(DtDataType *self, FILE *out)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));
    assert(out);

    dt_data_type_print(DT_ARRAY_TYPE(self)->data_type, out);
    fprintf(out, "[%d]", DT_STATIC_ARRAY_TYPE(self)->length);
}

static char *
dt_static_array_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    DtStaticArrayType *stat_arry = DT_STATIC_ARRAY_TYPE(self);

    if (stat_arry->mangled_name == NULL)
    {
        GString *str = g_string_new("G");

        g_string_append_printf(
            str, "%u%s",
            stat_arry->length,
            dt_data_type_get_mangled(
                dt_static_array_type_get_data_type(stat_arry)));

        stat_arry->mangled_name = g_string_free(str, FALSE);
    }

    return stat_arry->mangled_name;
}

static void
dt_static_array_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->get_mangled =
        dt_static_array_type_get_mangled;
    ((DtDataTypeClass *)klass)->do_print = dt_static_array_type_do_print;
}

