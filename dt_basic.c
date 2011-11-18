#include <stdbool.h>

#include "dt_basic.h"
#include "ir_basic_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             static table                                  *
 *---------------------------------------------------------------------------*/

/**
 * table of allowed implicit casts between basic types, where there is
 * no need to check value ranges
 *
 * column index is the source type, row index is target type,
 * if cell at impl_convs[target_type][source_type] == true, then implicit
 * conversation betweem source_type and target_type is allowed
 */
static bool impl_convs[last_basic_type][last_basic_type] =
{
            /* char   bool   byte   ubyte  short  ushort int    uint */
 /*  char  */ {true,  true,  true,  true,  false, false, false, false},
 /*  bool  */ {false, true,  false, false, false, false, false, false},
 /*  byte  */ {true,  true,  true,  true,  false, false, false, false},
 /* ubyte  */ {true,  true,  true,  true,  false, false, false, false},
 /* short  */ {true,  true,  true,  true,  true,  true,  false, false},
 /* ushort */ {true,  true,  true,  true,  true,  true,  false, false},
 /*  int   */ {true,  true,  true,  true,  true,  true,  true,  true},
 /*  uint  */ {true,  true,  true,  true,  true,  true,  true,  true}
};


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

static bool
dt_basic_is_type(DtDataType *type, basic_data_type_t basic_type);

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
                                    "DtBasicType",
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
dt_basic(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_BASIC, DtBasic);
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

UtRange *
dt_basic_get_value_range(DtBasic *self)
{
    assert(dt_is_basic(self));
    static UtRange *type_ranges[last_basic_type] =
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    if (type_ranges[self->data_type] == NULL)
    {
        switch (self->data_type)
        {
            case bool_type:
                type_ranges[self->data_type] = ut_range_new(0, 1);
                break;
            case char_type:
                type_ranges[self->data_type] = ut_range_new(0, 255);
                break;
            case byte_type:
                type_ranges[self->data_type] = ut_range_new(-128, 127);
                break;
            case ubyte_type:
                type_ranges[self->data_type] = ut_range_new(0, 255);
                break;
            case short_type:
                type_ranges[self->data_type] = ut_range_new(-32768, 32767);
                break;
            case ushort_type:
                type_ranges[self->data_type] = ut_range_new(0, 65535);
                break;
            case int_type:
                type_ranges[self->data_type] =
                                     ut_range_new(-2147483647 - 1, 2147483647);
                break;
            case uint_type:
                type_ranges[self->data_type] = ut_range_new(0, 4294967295u);
                break;
            default:
                /* unexpected data type */
                assert(false);
        }
    }

    return type_ranges[self->data_type];
}

bool
dt_basic_is_bool(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, bool_type);
}

bool
dt_basic_is_char(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, char_type);
}

bool
dt_basic_is_int(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, int_type);
}

bool
dt_basic_is_uint(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, uint_type);
}

bool
dt_basic_is_short(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, short_type);
}

bool
dt_basic_is_ushort(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, ushort_type);
}

bool
dt_basic_is_byte(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, byte_type);
}

bool
dt_basic_is_ubyte(DtDataType *data_type)
{
    return dt_basic_is_type(data_type, ubyte_type);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_basic_get_string(DtDataType *self)
{
    assert(dt_is_basic(self));

    char *str;
    switch (dt_basic(self)->data_type)
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

    switch (dt_basic(self)->data_type)
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

    switch (dt_basic(self)->data_type)
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

    switch (dt_basic(self)->data_type)
    {
        case int_type:
            return ir_expression(ir_basic_constant_new_int(0, 0));
        case uint_type:
            return ir_expression(ir_basic_constant_new_uint(0, 0));
        case short_type:
            return ir_expression(ir_basic_constant_new_short(0));
        case ushort_type:
            return ir_expression(ir_basic_constant_new_ushort(0));
        case bool_type:
            return ir_expression(ir_basic_constant_new_bool(false, 0));
        case char_type:
            return ir_expression(ir_basic_constant_new_char(255, 0));
        case byte_type:
            return ir_expression(ir_basic_constant_new_byte(0));
        case ubyte_type:
            return ir_expression(ir_basic_constant_new_ubyte(0));
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

    return dt_basic(self)->data_type == dt_basic(type)->data_type;
}

static bool
dt_basic_is_integral(DtDataType *self)
{
    assert(dt_is_basic(self));

    /* currently all supported basic types are integral */
    return true;
}

static bool
dt_basic_is_impl_conv(DtDataType *self, IrExpression *expression)
{
    assert(dt_is_basic(self));
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *expr_type = ir_expression_get_data_type(expression);

    if (!dt_is_basic(expr_type))
    {
        /* only basic types are implicitly convertible to basic types */
        return false;
    }

    if (impl_convs[dt_basic_get_data_type(dt_basic(self))]
                  [dt_basic_get_data_type(dt_basic(expr_type))])
    {
        /* a narrow type is converted to wider type */
        return true;
    }

    /* check is expression value range fits into target type */
    UtRange *type_range = dt_basic_get_value_range(dt_basic(self));
    UtRange *exp_range = ir_expression_get_value_range(expression);

    return ut_range_includes(type_range, exp_range);
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
    DT_DATA_TYPE_CLASS(klass)->is_impl_conv = dt_basic_is_impl_conv;
}

static bool
dt_basic_is_type(DtDataType *type, basic_data_type_t basic_type)
{
    if (!dt_is_basic(type))
    {
        return false;
    }

    return dt_basic_get_data_type(dt_basic(type)) == basic_type;
}
