#include <stdbool.h>

#include "dt_static_array.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_static_array_type_class_init(gpointer klass, gpointer dummy);

static char *
dt_static_array_type_get_string(DtDataType *self);

static char *
dt_static_array_type_get_mangled(DtDataType *self);

static guint
dt_static_array_type_get_size(DtDataType *self);

static IrExpression *
dt_static_array_type_get_init(DtDataType *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_static_array_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtStaticArrayClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_static_array_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtStaticArray),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_ARRAY,
                                    "DtStaticArrayTypeType",
                                    &info, 0);
    }
    return type;
}

DtStaticArray *
dt_static_array_new(DtDataType *data_type, guint32 length)
{
    DtStaticArray *obj;

    obj = g_object_new(DT_TYPE_STATIC_ARRAY, NULL);
    DT_ARRAY(obj)->data_type = data_type;
    obj->length = length;
    obj->string_of = NULL;
    obj->mangled_name = NULL;

    return obj;
}

DtDataType *
dt_static_array_get_data_type(DtStaticArray *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return dt_array_get_data_type(DT_ARRAY(self));
}

guint32
dt_static_array_get_length(DtStaticArray *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return self->length;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_static_array_type_get_string(DtDataType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    DtStaticArray *arry = DT_STATIC_ARRAY(self);

    if (arry->string_of == NULL)
    {
        arry->string_of =
            g_strdup_printf("%s[%uu]",
                dt_data_type_get_string(
                    dt_static_array_get_data_type(arry)),
                arry->length);
    }

    return arry->string_of;
}

static guint
dt_static_array_type_get_size(DtDataType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return dt_data_type_get_size(DT_ARRAY(self)->data_type) *
                                        DT_STATIC_ARRAY(self)->length;
}

static char *
dt_static_array_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    DtStaticArray *stat_arry = DT_STATIC_ARRAY(self);

    if (stat_arry->mangled_name == NULL)
    {
        GString *str = g_string_new("G");

        g_string_append_printf(
            str, "%u%s",
            stat_arry->length,
            dt_data_type_get_mangled(
                dt_static_array_get_data_type(stat_arry)));

        stat_arry->mangled_name = g_string_free(str, FALSE);
    }

    return stat_arry->mangled_name;
}

static IrExpression *
dt_static_array_type_get_init(DtDataType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return dt_data_type_get_init(DT_ARRAY(self)->data_type);
}

static void
dt_static_array_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->get_size = dt_static_array_type_get_size;
    ((DtDataTypeClass *)klass)->get_string = dt_static_array_type_get_string;
    ((DtDataTypeClass *)klass)->get_mangled =
        dt_static_array_type_get_mangled;
    ((DtDataTypeClass *)klass)->get_init = dt_static_array_type_get_init;
}
