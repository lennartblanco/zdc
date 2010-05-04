#include <stdbool.h>

#include "dt_basic_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_basic_type_class_init(gpointer klass, gpointer dummy);

static guint
dt_basic_type_get_size(DtDataType *self);

static char *
dt_basic_type_get_string(DtDataType *self);

static char *
dt_basic_type_get_mangled(DtDataType *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_basic_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtBasicTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_basic_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtBasicType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtBasicTypeType",
                                    &info, 0);
    }
    return type;
}

DtBasicType * 
dt_basic_type_new(basic_data_type_t data_type)
{
    DtBasicType *basic_type;

    basic_type = g_object_new(DT_TYPE_BASIC_TYPE, NULL);
    basic_type->data_type = data_type;

    return basic_type;
}

basic_data_type_t
dt_basic_type_get_data_type(DtBasicType *self)
{
    assert(DT_IS_BASIC_TYPE(self));

    return self->data_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_basic_type_get_string(DtDataType *self)
{
    assert(DT_IS_BASIC_TYPE(self));

    char *str;
    switch (DT_BASIC_TYPE(self)->data_type)
    {
        case int_type:
            str = "int";
            break;
        case uint_type:
            str = "uint";
            break;
        case void_type:
            str = "void";
            break;
        case bool_type:
            str = "bool";
            break;
        case char_type:
            str = "char";
            break;
        default:
            /* unexpected basic data type */
            assert(false);
    }
    return str;
}

static guint
dt_basic_type_get_size(DtDataType *self)
{
    assert(DT_IS_BASIC_TYPE(self));

    switch (DT_BASIC_TYPE(self)->data_type)
    {
        case int_type:
        case uint_type:
            return 4;
        case char_type:
        case bool_type:
            return 1;
        default:
            /* unexpected basic data type */
            assert(false);
    }
}

static char *
dt_basic_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_BASIC_TYPE(self));

    switch (DT_BASIC_TYPE(self)->data_type)
    {
        case int_type:
            return "i";
        case uint_type:
            return "k";
        case void_type:
            return "v";
        case bool_type:
            return "b";
        case char_type:
            return "a";
        default:
            /* unexpected basic data type */
            assert(false);
    }
    
}

static void
dt_basic_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->get_size = dt_basic_type_get_size;
    ((DtDataTypeClass *)klass)->get_string = dt_basic_type_get_string;
    ((DtDataTypeClass *)klass)->get_mangled = dt_basic_type_get_mangled;
}

