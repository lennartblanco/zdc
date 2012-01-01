#include "dt_data_type.h"
#include "dt_enum.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_data_type_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtDataTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtDataType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "DtDataTypeType",
                                    &info, 0);
    }
    return type;
}

void
dt_data_type_set_immutable(DtDataType *self, bool is_immutable)
{
    assert(DT_IS_DATA_TYPE(self));

    self->immutable = is_immutable;
}

bool
dt_data_type_is_immutable(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    return self->immutable;
}

guint
dt_data_type_get_size(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_DATA_TYPE_GET_CLASS(self)->get_size != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->get_size(self);
}

/**
 * @return the source string representation of this data type.
 */
char *
dt_data_type_get_string(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_DATA_TYPE_GET_CLASS(self)->get_string != NULL);

    char *str = DT_DATA_TYPE_GET_CLASS(self)->get_string(self);

    if (self->immutable)
    {
        char *tmp = g_strdup_printf("immutable(%s)", str);
        str = tmp;
    }

    return str;
}

char *
dt_data_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_DATA_TYPE_GET_CLASS(self)->get_mangled != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->get_mangled(self);
}

IrExpression *
dt_data_type_get_init(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_DATA_TYPE_GET_CLASS(self)->get_init != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->get_init(self);
}

bool
dt_data_type_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(DT_IS_DATA_TYPE(type));
    assert(DT_DATA_TYPE_GET_CLASS(self)->is_same != NULL);

    return DT_DATA_TYPE_GET_CLASS(self)->is_same(self, type);
}

bool
dt_data_type_is_integral(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    /* by default a data type is not integral */
    if (DT_DATA_TYPE_GET_CLASS(self)->is_integral == NULL)
    {
        /* method not overridden, return default value */
        return false;
    }

    return DT_DATA_TYPE_GET_CLASS(self)->is_integral(self);
}

bool
dt_data_type_is_impl_conv(DtDataType *self, IrExpression *expression)
{
    assert(DT_IS_DATA_TYPE(self));
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *expr_type = ir_expression_get_data_type(expression);

    if (dt_data_type_is_same(self, expr_type))
    {
        /* all types are always implicitly convertible to them self */
        return true;
    }
    else if (dt_is_enum(expr_type) &&
             dt_data_type_is_same(self,
                                  dt_enum_get_base_type(dt_enum(expr_type))))
    {
        /* enum type are implicitly convertible to it's base type */
        return true;
    }

    assert(DT_DATA_TYPE_GET_CLASS(self)->is_impl_conv != NULL);
    return DT_DATA_TYPE_GET_CLASS(self)->is_impl_conv(self, expression);
}
