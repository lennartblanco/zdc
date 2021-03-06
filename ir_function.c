#include <stdbool.h>
#include <string.h>

#include "ir_function.h"
#include "ir_variable.h"
#include "ir_module.h"

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

/**
 * Generate mangled name for function with D linkage.
 */
static char *
get_d_mangled_name(IrFunction *self);

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

IrFunction *
ir_function(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_FUNCTION, IrFunction);
}

char *
ir_function_get_name(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    return ir_symbol_get_name(ir_symbol(self));
}

char *
ir_function_get_mangled_name(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    if (self->mangled_name == NULL)
    {
        switch (self->linkage_type)
        {
            case ir_d_linkage:
                self->mangled_name = get_d_mangled_name(self);
                break;
            case ir_c_linkage:
                /* C functions names are not mangled */
                self->mangled_name = ir_function_get_name(self);
                break;
            default:
                /* unexpected linkage type */
                assert(false);
        }
    }

    return self->mangled_name;
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
ir_function_set_this_param(IrFunction *self, IrVariable *this_param)
{
    assert(IR_IS_FUNCTION(self));
    assert(IR_IS_VARIABLE(this_param));

    self->this_param = this_param;
}

IrVariable *
ir_function_get_this_param(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));

    return self->this_param;
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
ir_function_set_property(GObject *object,
                         guint property_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
    assert(IR_IS_FUNCTION(object));

    IrFunction *func = ir_function(object);

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

static char *
get_d_mangled_name(IrFunction *self)
{
    assert(IR_IS_FUNCTION(self));
    assert(self->linkage_type == ir_d_linkage);

    GSList *i;
    char *func_name;

    GString *str = g_string_new("_D");

    g_string_append(str,
        ir_scope_get_mangle_prefix(
            ir_symbol_get_scope(ir_symbol(self))));

    func_name = ir_function_get_name(ir_function(self));
    g_string_append_printf(str, "%zu%s%sF",
                           strlen(func_name), func_name,
                           self->this_param != NULL ? "M" : "");

    /* add parameter types mangled names */
    i = ir_function_get_parameters(self);
    for (; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_VARIABLE(i->data));

        IrVariable *var = ir_variable(i->data);

        if (ir_variable_is_ref(var))
        {
            g_string_append(str, "K");
        }

        g_string_append(str,
                        dt_data_type_get_mangled(
                            ir_variable_get_data_type(var)));
    }

    g_string_append_printf(
        str, "Z%s",
        dt_data_type_get_mangled(ir_function_get_return_type(self)));

    return g_string_free(str, FALSE);
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
}
