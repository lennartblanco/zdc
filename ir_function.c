#include <stdbool.h>

#include "ir_function.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    IR_FUNCTION_LINKAGE_TYPE = 1
};


#define IR_TYPE_LINKAGE_ATTR ir_linkage_attr_get_type()

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_function_class_init(gpointer klass, gpointer foo);

static void
ir_function_do_print(IrSymbol *self, FILE *out, int indention);

static void
ir_function_set_property(GObject *object,
                         guint property_id,
                         const GValue *value,
                         GParamSpec *pspec);

static void
ir_function_get_property(GObject *object,
                         guint property_id,
                         GValue *value,
                         GParamSpec *pspec);

static GType
ir_linkage_attr_get_type(void);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrFunctionClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_function_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFunction),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "IrFunctionType",
                                    &info, 0);
    }
    return type;
}

char *
ir_function_get_name(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    return ir_symbol_get_name(IR_SYMBOL(self));
}

ir_linkage_type_t
ir_function_get_linkage(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    return self->linkage_type;
}

void
ir_function_set_parameters(IrFunction *self, GSList *parameters)
{
    assert(IR_IS_FUNCTION(self));

    self->parameters = parameters;
}

GSList *
ir_function_get_parameters(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    return self->parameters;
}

void
ir_function_set_return_type(IrFunction *self, DtDataType *return_type)
{
    assert(IR_IS_FUNCTION(self));
    assert(DT_IS_DATA_TYPE(return_type));

    self->return_type = return_type;
}

DtDataType *
ir_function_get_return_type(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    return self->return_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_function_do_print(IrSymbol *self, FILE *out, int indention)
{
    assert(self);
    assert(out);

    IrFunction *func = IR_FUNCTION(self);

    fprintf_indent(out, indention,
            "function [%p]\n"
            "  name: '%s'\n"
            "  return_type: ",
            self,
            ir_symbol_get_name(self));

    ast_node_print(XDP_AST_NODE(func->return_type), out);
    fprintf_indent(out, indention, "\n  parameters: ");

    GSList *i = func->parameters;
    for (;i != NULL; i = g_slist_next(i))
    {
        ast_node_print(XDP_AST_NODE(i->data), out);
        fprintf(out, "%s", g_slist_next(i) != NULL ? ", " : "");
    }
}

static void
ir_function_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ir_function_set_property;
    gobject_class->get_property = ir_function_get_property;

    /*
     * install 'name' property 
     */
    pspec = g_param_spec_enum("ir-function-linkage-type",
                              "ir function linkage",
                              "the linkage type attribute of the function",
                              IR_TYPE_LINKAGE_ATTR,
                              ir_d_linkage,
                              G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_FUNCTION_LINKAGE_TYPE,
                                    pspec);


    ((IrSymbolClass *)klass)->do_print = ir_function_do_print;
}

static void
ir_function_set_property(GObject *object,
                         guint property_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
    assert(IR_IS_FUNCTION(object));

    IrFunction *func = IR_FUNCTION(object);

    /* we only have one property */
    assert(property_id == IR_FUNCTION_LINKAGE_TYPE);

    func->linkage_type = g_value_get_enum(value);
}

static void
ir_function_get_property(GObject *object,
                         guint property_id,
                         GValue *value,
                         GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}


static GType
ir_linkage_attr_get_type(void)
{
  static GType linkage_attr_type = 0;
  static GEnumValue linkage_attr[] =
  {
      {ir_d_linkage, "D", "D linkage"},
      {ir_c_linkage, "C", "C linkage"},
      {0, NULL, NULL}
  };

  if (linkage_attr_type == 0)
  {
      linkage_attr_type =
          g_enum_register_static("IrLinkageAttrType", linkage_attr);
  }
  return linkage_attr_type;

}
