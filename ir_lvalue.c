#include <stdbool.h>

#include "ir_lvalue.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    IR_LVALUE_SYMBOL_NAME = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_lvalue_class_init(gpointer klass, gpointer foo);

static void
ir_lvalue_set_property(GObject *object,
                              guint property_id,
                              const GValue *value,
                              GParamSpec *pspec);

static void
ir_lvalue_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_lvalue_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrLvalueClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_lvalue_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrLvalue),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrLvalueType",
                                    &info, 0);
    }
    return type;
}

char *
ir_lvalue_get_name(IrLvalue *self)
{
    assert(IR_IS_LVALUE(self));

    return self->symbol_name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_lvalue_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ir_lvalue_set_property;
    gobject_class->get_property = ir_lvalue_get_property;

    /*
     * install 'name' property 
     */
    pspec = g_param_spec_string("ir-lvalue-symbol-name",
                                "ir lvalue symbol name",
                                "the name of the symbol",
                                "no-name-set" /* default value */,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_LVALUE_SYMBOL_NAME,
                                    pspec);
}

static void
ir_lvalue_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec)
{
    /* we only have one property */
    assert(property_id == IR_LVALUE_SYMBOL_NAME);
    IrLvalue *lvalue = IR_LVALUE(object);

    lvalue->symbol_name = g_value_dup_string(value);    
}

static void
ir_lvalue_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

