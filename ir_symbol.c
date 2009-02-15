#include <stdbool.h>
#include <strings.h>

#include "ir_symbol.h"
#include "utils.h"

#include <assert.h>
/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    IR_VARIABLE_NAME = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_symbol_class_init(gpointer klass, gpointer foo);

static void
ir_symbol_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec);

static void
ir_symbol_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec);

static void
ir_symbol_do_print(IrSymbol *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_symbol_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrSymbolClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_symbol_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrSymbol),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrSymbolType",
                                    &info, 0);
    }
    return type;
}

char *
ir_symbol_get_name(IrSymbol *self)
{
    assert(self);
    assert(IR_IS_SYMBOL(self));

    return self->name;
}

void
ir_symbol_print(IrSymbol *self, FILE *out, int indention)
{
    IR_SYMBOL_GET_CLASS(self)->do_print(self, out, indention);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_symbol_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec)
{
    /* we only have one property */
    assert(property_id == IR_VARIABLE_NAME);
    IrSymbol *sym = IR_SYMBOL(object);

    sym->name = g_value_dup_string(value);
}

static void
ir_symbol_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static void
ir_symbol_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ir_symbol_set_property;
    gobject_class->get_property = ir_symbol_get_property;

    /*
     * install 'name' property 
     */
    pspec = g_param_spec_string("ir-symbol-name",
                                "ir symbol name",
                                "the name of the symbol",
                                "no-name-set" /* default value */,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_VARIABLE_NAME,
                                    pspec);

    /*
     * set default implementation of print() method
     */
    ((IrSymbolClass *)klass)->do_print = ir_symbol_do_print;
}

static void
ir_symbol_do_print(IrSymbol *self, FILE *out, int indention)
{
    fprintf_indent(out, indention, "symbol [%p] name: '%s'\n",
                   self, self->name);
}
