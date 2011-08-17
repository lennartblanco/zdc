#include "dt_data_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    DT_DATA_TYPE_LINE_NUMBER = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_data_type_class_init(gpointer klass, gpointer foo);

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
        dt_data_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtDataType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "DtDataTypeType",
                                    &info, 0);
    }
    return type;
}

void
dt_data_type_set_immutable(DtDataType *self, bool is_immutable)
{
    assert(DT_IS_DATA_TYPE(self));

    self->immutable = true;
}

bool
dt_data_type_is_immutalbe(DtDataType *self)
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

guint
dt_data_type_get_line_num(DtDataType *self)
{
    assert(DT_IS_DATA_TYPE(self));

    return (DT_DATA_TYPE(self)->line_number);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
dt_data_type_set_property(GObject *object,
                          guint property_id,
                          const GValue *value,
                          GParamSpec *pspec)
{
    assert(DT_IS_DATA_TYPE(object));
    /* we only have one property */
    assert(property_id == DT_DATA_TYPE_LINE_NUMBER);

    DT_DATA_TYPE(object)->line_number = g_value_get_uint(value);
}

static void
dt_data_type_get_property(GObject *object,
                          guint property_id,
                          GValue *value,
                          GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static void
dt_data_type_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = dt_data_type_set_property;
    gobject_class->get_property = dt_data_type_get_property;

    /*
     * install 'line-number' property
     */
    pspec = g_param_spec_uint("dt-data-type-line-number",
                              "dt data type line number",
                              "source file line number",
                              0,          /* min value */
                              G_MAXUINT,  /* max value */
                              0,          /* default value */
                              G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    DT_DATA_TYPE_LINE_NUMBER,
                                    pspec);
}
