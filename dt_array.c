#include <stdbool.h>

#include "dt_array.h"
#include "ir_null.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static guint
dt_array_type_get_size(DtDataType *self);

static char *
dt_array_type_get_string(DtDataType *self);

static char *
dt_array_type_get_mangled(DtDataType *self);

static void
dt_array_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_array_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtArrayClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_array_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtArray),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtArrayType",
                                    &info, 0);
    }
    return type;
}

DtArray *
dt_array_new(DtDataType *data_type)
{
    DtArray *obj;

    obj = g_object_new(DT_TYPE_ARRAY, NULL);
    obj->data_type = data_type;
    obj->string_of = NULL;
    obj->mangled_name = NULL;

    return obj;
}

DtDataType *
dt_array_get_data_type(DtArray *self)
{
    assert(DT_IS_ARRAY(self));

    return self->data_type;
}

guint
dt_array_get_element_size(DtArray *self)
{
    assert(DT_IS_ARRAY(self));

    return dt_data_type_get_size(self->data_type);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_array_type_get_string(DtDataType *self)
{
    assert(DT_IS_ARRAY(self));

    DtArray *arry = DT_ARRAY(self);

    if (arry->string_of == NULL)
    {
        arry->string_of =
            g_strdup_printf("%s[]", 
                dt_data_type_get_string(dt_array_get_data_type(arry)));

    }

    return arry->string_of;
}

static char *
dt_array_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_ARRAY(self));

    DtArray *arry = DT_ARRAY(self);

    if (arry->mangled_name == NULL)
    {
        arry->mangled_name =
            g_strdup_printf("A%s", 
                dt_data_type_get_mangled(dt_array_get_data_type(arry)));

    }

    return arry->mangled_name;
}

static guint
dt_array_type_get_size(DtDataType *self)
{
    assert(DT_IS_ARRAY(self));

    return 8;
}

static IrExpression *
dt_array_type_get_init(DtDataType *self)
{
    assert(DT_IS_ARRAY(self));

    return IR_EXPRESSION(ir_null_new());
}

static void
dt_array_type_class_init(gpointer klass, gpointer dummy)
{
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_array_type_get_size;
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_array_type_get_string;
    DT_DATA_TYPE_CLASS(klass)->get_mangled = dt_array_type_get_mangled;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_array_type_get_init;
}

