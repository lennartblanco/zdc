#include <string.h>

#include "ir_property.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_property_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_property_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrPropertyClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_property_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrProperty),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrPropertyType",
                                    &info, 0);
    }
    return type;
}

IrProperty *
ir_property_new(IrExpression *exp, const char *name, guint line_number)
{
    assert(IR_IS_EXPRESSION(exp));
    assert(name);

    ir_property_id_t prop_id;
    IrProperty *obj;

    if (strcmp(IR_PROP_INIT, name) == 0)
    {
        prop_id = ir_prop_init;
    }
    else if (strcmp(IR_PROP_SIZEOF, name) == 0)
    {
        prop_id = ir_prop_sizeof;
    }
    else if (strcmp(IR_PROP_LENGTH, name) == 0)
    {
        prop_id = ir_prop_length;
    }
    else
    {
        /* unknow property name */
        return NULL;
    }

    obj = g_object_new(IR_TYPE_PROPERTY,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->exp = exp;
    obj->id = prop_id;

    return obj;
}

void
ir_property_set_expression(IrProperty *self, IrExpression *exp)
{
    assert(IR_IS_PROPERTY(self));
    assert(IR_IS_EXPRESSION(exp));

    self->exp = exp;
}

IrExpression *
ir_property_get_expression(IrProperty *self)
{
    assert(IR_IS_PROPERTY(self));

    return self->exp;
}

ir_property_id_t
ir_property_get_id(IrProperty *self)
{
    assert(IR_IS_PROPERTY(self));

    return self->id;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_property_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_PROPERTY(self));
    /*
     * only length properties so far are run-time,
     * it's an error to try to fetch type
     */
    assert(IR_PROPERTY(self)->id == ir_prop_length);
    return types_get_uint_type();
}

static void
ir_property_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_property_do_get_data_type;
}
