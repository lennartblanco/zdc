#include <stdbool.h>

#include "dt_array_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_array_type_do_print(DtDataType *self, FILE *out);

static void
dt_array_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_array_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtArrayTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_array_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtArrayType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtArrayTypeType",
                                    &info, 0);
    }
    return type;
}

DtArrayType *
dt_array_type_new(DtDataType *data_type)
{
    DtArrayType *obj;

    obj = g_object_new(DT_TYPE_ARRAY_TYPE, NULL);
    obj->data_type = data_type;

    return obj;
}

DtDataType *
dt_array_type_get_data_type(DtArrayType *self)
{
    assert(DT_IS_ARRAY_TYPE(self));

    return self->data_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
dt_array_type_do_print(DtDataType *self, FILE *out)
{
    assert(DT_IS_ARRAY_TYPE(self));
    assert(out);

    dt_data_type_print(DT_ARRAY_TYPE(self)->data_type, out);
    fprintf(out, "[]");
}

static void
dt_array_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->do_print = dt_array_type_do_print;
}

