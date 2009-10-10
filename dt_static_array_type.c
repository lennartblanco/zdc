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
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtStaticArrayTypeType",
                                    &info, 0);
    }
    return type;
}

DtStaticArrayType *
dt_static_array_type_new(basic_data_type_t data_type, int length)
{
    DtStaticArrayType *obj;

    obj = g_object_new(DT_TYPE_STATIC_ARRAY_TYPE, NULL);
    obj->data_type = data_type;
    obj->length = length;

    return obj;
}

basic_data_type_t
dt_static_array_type_get_data_type(DtStaticArrayType *self)
{
    assert(DT_IS_STATIC_ARRAY_TYPE(self));

    return self->data_type;
}

int
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

    char *str;
    DtStaticArrayType *obj = DT_STATIC_ARRAY_TYPE(self);

    switch(obj->data_type)
    {
        case int_type:
            str = "int";
            break;
        case void_type:
            str = "void";
            break;
        case bool_type:
            str = "bool";
            break;
        default:
            /* unexpected basic data type */
            assert(false);
    }
    fprintf(out, "%s[%d]", str, obj->length);
}

static void
dt_static_array_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->do_print = dt_static_array_type_do_print;
}

