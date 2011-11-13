#include <stdbool.h>
#include <strings.h>

#include "ir_symbol.h"
#include "ir_module.h"
#include "utils.h"

#include <assert.h>
/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    IR_SYMBOL_NAME = 1,
    IR_SYMBOL_PARENT_MODULE
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_symbol_init(IrSymbol *self, gpointer klass);

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
ir_symbol_do_print(IrNode *self, FILE *out, int indention);

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
        (GInstanceInitFunc) ir_symbol_init    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrSymbolType",
                                    &info, 0);
    }
    return type;
}

char *
ir_symbol_get_name(IrSymbol *self)
{
    assert(IR_IS_SYMBOL(self));

    return self->name;
}

char *
ir_symbol_get_fqname(IrSymbol *self)
{
    assert(IR_IS_SYMBOL(self));
    assert(IR_IS_MODULE(self->parent_module));

    if (self->fq_name == NULL)
    {
        GString *str =
            g_string_new(ir_module_get_fqname(self->parent_module));
        g_string_append(str, ".");
        g_string_append(str, self->name);

        self->fq_name = g_string_free(str, false);
    }

    return self->fq_name;
}

IrModule *
ir_symbol_get_parent_module(IrSymbol *self)
{
    assert(IR_IS_SYMBOL(self));
    assert(IR_IS_MODULE(self->parent_module));

    return self->parent_module;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_symbol_init(IrSymbol *self, gpointer klass)
{
    self->fq_name = NULL;
}

static void
ir_symbol_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec)
{
    IrSymbol *sym = IR_SYMBOL(object);

    switch (property_id)
    {
        case IR_SYMBOL_NAME:
            sym->name = g_value_dup_string(value);
            break;
        case IR_SYMBOL_PARENT_MODULE:
            sym->parent_module = g_value_get_object(value);
            break;
        default:
            /* unexpected property id */
            assert(false);
    }
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
                                G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_SYMBOL_NAME,
                                    pspec);

    /*
     * install 'parent module' property 
     */
    pspec = g_param_spec_object("ir-symbol-parent-module",
                                "ir symbol parent module",
                                "the parent module of the symbol",
                                IR_TYPE_MODULE,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_SYMBOL_PARENT_MODULE,
                                    pspec);

    /*
     * set default implementation of print() method for a symbol
     */
    IR_NODE_CLASS(klass)->do_print = ir_symbol_do_print;
}

static void
ir_symbol_do_print(IrNode *self, FILE *out, int indention)
{
    fprintf_indent(out, indention, "symbol [%p] name: '%s'\n",
                   self, IR_SYMBOL(self)->name);
}
