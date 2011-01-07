#include "dt_pointer.h"
#include "ir_null.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_pointer_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_pointer_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtPointerClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_pointer_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtPointer),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtPointerType",
                                    &info, 0);
    }
    return type;
}

DtPointer *
dt_pointer_new(DtDataType *base_type)
{
    DtPointer *obj;

    obj = g_object_new(DT_TYPE_POINTER, NULL);
    obj->base_type = base_type;

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

//static char *
//dt_array_type_get_string(DtDataType *self)
//{
//    assert(DT_IS_ARRAY_TYPE(self));
//
//    DtArrayType *arry = DT_ARRAY_TYPE(self);
//
//    if (arry->string_of == NULL)
//    {
//        arry->string_of =
//            g_strdup_printf("%s[]",
//                dt_data_type_get_string(dt_array_type_get_data_type(arry)));
//
//    }
//
//    return arry->string_of;
//}
//
static char *
dt_pointer_get_mangled(DtDataType *self)
{
    assert(DT_IS_POINTER(self));

    DtPointer *ptr = DT_POINTER(self);

    if (ptr->mangled_name == NULL)
    {
        ptr->mangled_name =
            g_strdup_printf("P%s",
                dt_data_type_get_mangled(ptr->base_type));

    }

    return ptr->mangled_name;
}

//static guint
//dt_array_type_get_size(DtDataType *self)
//{
//    assert(DT_IS_ARRAY_TYPE(self));
//
//    return 8;
//}
//
static IrExpression *
dt_pointer_get_init(DtDataType *self)
{
    assert(DT_IS_POINTER(self));

    return IR_EXPRESSION(ir_null_new());
}

static bool
dt_pointer_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_POINTER(self));
    assert(DT_IS_DATA_TYPE(type));


    if (!DT_IS_POINTER(type))
    {
        return false;
    }

    return dt_data_type_is_same(DT_POINTER(self)->base_type,
                                DT_POINTER(type)->base_type);
}

static void
dt_pointer_class_init(gpointer klass, gpointer dummy)
{
//    ((DtDataTypeClass *)klass)->get_size = dt_array_type_get_size;
//    ((DtDataTypeClass *)klass)->get_string = dt_array_type_get_string;
    ((DtDataTypeClass *)klass)->get_mangled = dt_pointer_get_mangled;
    ((DtDataTypeClass *)klass)->get_init = dt_pointer_get_init;
    ((DtDataTypeClass *)klass)->is_same = dt_pointer_is_same;
}

