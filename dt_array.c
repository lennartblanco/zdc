#include <stdbool.h>

#include "dt_array.h"
#include "ir_null.h"
#include "ir_array_literal.h"
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
dt_array(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), DT_TYPE_ARRAY, DtArray);
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
dt_array_get_element_type(DtArray *self)
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

    DtArray *arry = dt_array(self);

    if (arry->string_of == NULL)
    {
        arry->string_of =
            g_strdup_printf("%s[]", 
                dt_data_type_get_string(dt_array_get_element_type(arry)));

    }

    return arry->string_of;
}

static char *
dt_array_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_ARRAY(self));

    DtArray *arry = dt_array(self);

    if (arry->mangled_name == NULL)
    {
        arry->mangled_name =
            g_strdup_printf("A%s", 
                dt_data_type_get_mangled(dt_array_get_element_type(arry)));

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

    return ir_expression(ir_null_new());
}

static bool
dt_array_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_ARRAY(self));
    assert(DT_IS_DATA_TYPE(type));

    if (!DT_IS_ARRAY(type))
    {
        return false;
    }

    return
        dt_data_type_is_same(dt_array_get_element_type(dt_array(self)),
                             dt_array_get_element_type(dt_array(type)));

}

static bool
dt_array_is_impl_conv(DtDataType *self, IrExpression *expression)
{
    assert(DT_IS_ARRAY(self));
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *expr_type = ir_expression_get_data_type(expression);

    if (!DT_IS_ARRAY(expr_type))
    {
        return false;
    }

    /* the expression should be of array literal type */
    assert(ir_is_array_literal(expression));

    /*
     * if all array literal's values are implicitly convertable to
     * our element type, then the whole literal is convertable to this type
     */
    DtDataType *element_type = dt_array_get_element_type(dt_array(self));
    GSList *i = ir_array_literal_get_values(ir_array_literal(expression));
    for (; i != NULL; i = g_slist_next(i))
    {
        if (!dt_data_type_is_impl_conv(element_type, ir_expression(i->data)))
        {
            return false;
        }
    }

    return true;
}

static void
dt_array_type_class_init(gpointer klass, gpointer dummy)
{
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_array_type_get_size;
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_array_type_get_string;
    DT_DATA_TYPE_CLASS(klass)->get_mangled = dt_array_type_get_mangled;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_array_type_get_init;
    DT_DATA_TYPE_CLASS(klass)->is_same = dt_array_is_same;
    DT_DATA_TYPE_CLASS(klass)->is_impl_conv = dt_array_is_impl_conv;
}

