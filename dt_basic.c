#include <stdbool.h>

#include "dt_basic.h"
#include "ir_basic_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_basic_class_init(gpointer klass, gpointer dummy);

static guint
dt_basic_get_size(DtDataType *self);

static char *
dt_basic_get_string(DtDataType *self);

static char *
dt_basic_get_mangled(DtDataType *self);

static IrExpression *
dt_basic_get_init(DtDataType *self);

static bool
dt_basic_is_same(DtDataType *self, DtDataType *type);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_basic_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtBasicClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_basic_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtBasic),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtBasicTypeType",
                                    &info, 0);
    }
    return type;
}

bool
dt_is_basic(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_TYPE((obj), DT_TYPE_BASIC);
}

DtBasic *
dt_basic_new(basic_data_type_t data_type)
{
    DtBasic *basic_type;

    basic_type = g_object_new(DT_TYPE_BASIC, NULL);
    basic_type->data_type = data_type;

    return basic_type;
}

basic_data_type_t
dt_basic_get_data_type(DtBasic *self)
{
    assert(dt_is_basic(self));

    return self->data_type;
}

bool
dt_basic_is_signed(DtBasic *self)
{
    bool is_signed;

    switch (self->data_type)
    {
        case int_type:
            is_signed = true;
            break;
        case char_type:
        case uint_type:
            is_signed = false;
            break;
        default:
            /* signess not defined for this data type */
            assert(false);
    }

    return is_signed;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_basic_get_string(DtDataType *self)
{
    assert(dt_is_basic(self));

    char *str;
    switch (DT_BASIC(self)->data_type)
    {
        case int_type:
            str = "int";
            break;
        case uint_type:
            str = "uint";
            break;
        case short_type:
            str = "short";
            break;
        case ushort_type:
            str = "ushort";
            break;
        case byte_type:
            str = "byte";
            break;
        case ubyte_type:
            str = "ubyte";
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
dt_basic_get_size(DtDataType *self)
{
    assert(dt_is_basic(self));

    switch (DT_BASIC(self)->data_type)
    {
        case int_type:
        case uint_type:
            return 4;
        case short_type:
        case ushort_type:
            return 2;
        case byte_type:
        case ubyte_type:
        case char_type:
        case bool_type:
            return 1;
        default:
            /* unexpected basic data type */
            assert(false);
    }
}

static char *
dt_basic_get_mangled(DtDataType *self)
{
    assert(dt_is_basic(self));

    switch (DT_BASIC(self)->data_type)
    {
        case int_type:
            return "i";
        case uint_type:
            return "k";
        case short_type:
            return "s";
        case ushort_type:
            return "t";
        case byte_type:
            return "g";
        case ubyte_type:
            return "h";
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

static IrExpression *
dt_basic_get_init(DtDataType *self)
{
    assert(dt_is_basic(self));

    switch (DT_BASIC(self)->data_type)
    {
        case int_type:
            return IR_EXPRESSION(ir_basic_constant_new_int(0, 0));
        case uint_type:
            return IR_EXPRESSION(ir_basic_constant_new_uint(0, 0));
        case short_type:
            return IR_EXPRESSION(ir_basic_constant_new_short(0));
        case ushort_type:
            return IR_EXPRESSION(ir_basic_constant_new_ushort(0));
        case bool_type:
            return IR_EXPRESSION(ir_basic_constant_new_bool(false, 0));
        case char_type:
            return IR_EXPRESSION(ir_basic_constant_new_char(255, 0));
        case byte_type:
            return IR_EXPRESSION(ir_basic_constant_new_byte(0));
        case ubyte_type:
            return IR_EXPRESSION(ir_basic_constant_new_ubyte(0));
        default:
            assert(false);
    }
}

static bool
dt_basic_is_same(DtDataType *self, DtDataType *type)
{
    assert(dt_is_basic(self));
    assert(DT_IS_DATA_TYPE(type));

    if (!dt_is_basic(type))
    {
        return false;
    }

    return DT_BASIC(self)->data_type ==
           DT_BASIC(type)->data_type;
}

static bool
dt_basic_is_integral(DtDataType *self)
{
    assert(dt_is_basic(self));

    /* currently only void basic type is not integral */
    return DT_BASIC(self)->data_type != void_type;
}

static void
dt_basic_class_init(gpointer klass, gpointer dummy)
{
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_basic_get_size;
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_basic_get_string;
    DT_DATA_TYPE_CLASS(klass)->get_mangled = dt_basic_get_mangled;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_basic_get_init;
    DT_DATA_TYPE_CLASS(klass)->is_same = dt_basic_is_same;
    DT_DATA_TYPE_CLASS(klass)->is_integral = dt_basic_is_integral;
}

