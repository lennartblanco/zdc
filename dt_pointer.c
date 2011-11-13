#include "dt_pointer.h"
#include "dt_void.h"
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

    assert(DT_IS_DATA_TYPE(base_type));

    obj = g_object_new(DT_TYPE_POINTER, NULL);
    obj->base_type = base_type;

    return obj;
}

DtDataType *
dt_pointer_get_base_type(DtPointer *self)
{
    assert(DT_IS_POINTER(self));

    return self->base_type;
}

void
dt_pointer_set_base_type(DtPointer *self, DtDataType *base_type)
{
    assert(DT_IS_POINTER(self));
    assert(DT_IS_DATA_TYPE(base_type));

    self->base_type = base_type;
}

guint
dt_pointer_get_base_type_size(DtPointer *self)
{
    assert(DT_IS_POINTER(self));
    assert(DT_IS_DATA_TYPE(self->base_type));

    return dt_data_type_get_size(self->base_type);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_pointer_get_string(DtDataType *self)
{
    assert(DT_IS_POINTER(self));

    DtPointer *ptr = DT_POINTER(self);

    if (ptr->string_of == NULL)
    {
        ptr->string_of =
            g_strdup_printf("%s*",
                dt_data_type_get_string(dt_pointer_get_base_type(ptr)));

    }

    return ptr->string_of;
}

static char *
dt_pointer_get_mangled(DtDataType *self)
{
    assert(DT_IS_POINTER(self));

    DtPointer *ptr = DT_POINTER(self);

    if (ptr->mangled_name == NULL)
    {
        ptr->mangled_name =
            g_strdup_printf("P%s",
                dt_data_type_get_mangled(dt_pointer_get_base_type(ptr)));

    }

    return ptr->mangled_name;
}

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

static guint
dt_pointer_get_size(DtDataType *self)
{
    assert(DT_IS_POINTER(self));

    /* hard-code pointers to 4 bytes for all platforms for now */
    return 4;
}

static bool
dt_pointer_is_impl_conv(DtDataType *self, IrExpression *expression)
{
    assert(DT_IS_POINTER(self));
    assert(IR_IS_EXPRESSION(expression));

    if (DT_IS_VOID(dt_pointer_get_base_type(DT_POINTER(self))))
    {
        /* all pointer types are implicitly convertible to void* */
        return DT_IS_POINTER(ir_expression_get_data_type(expression));
    }

    return false;
}

static void
dt_pointer_class_init(gpointer klass, gpointer dummy)
{
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_pointer_get_string;
    DT_DATA_TYPE_CLASS(klass)->get_mangled = dt_pointer_get_mangled;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_pointer_get_init;
    DT_DATA_TYPE_CLASS(klass)->is_same = dt_pointer_is_same;
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_pointer_get_size;
    DT_DATA_TYPE_CLASS(klass)->is_impl_conv = dt_pointer_is_impl_conv;
}

