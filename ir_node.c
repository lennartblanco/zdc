#include <stdbool.h>

#include "ir_node.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    IR_NDDE_LINE_NUMBER = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_node_class_init(gpointer klass, gpointer foo);

static void
ir_node_set_property(GObject *object,
                     guint property_id,
                     const GValue *value,
                     GParamSpec *pspec);

static void
ir_node_get_property(GObject *object,
                     guint property_id,
                     GValue *value,
                     GParamSpec *pspec);

static void
ir_node_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_node_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (IrNodeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_node_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrNode),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrNodeType",
                                    &info, 0);
    }
    return type;
}

guint
ir_node_get_line_num(void *self)
{
    assert(IR_IS_NODE(self));

    return (IR_NODE(self)->line_number);
}

void
ir_node_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_NODE(self));
    assert(out);

    IR_NODE_GET_CLASS(self)->do_print(self, out, indention);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_node_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ir_node_set_property;
    gobject_class->get_property = ir_node_get_property;

    /*
     * install 'line-number' property 
     */
    pspec = g_param_spec_uint("ir-node-line-number",
                              "ir node line number",
                              "source file line number",
                              0,          /* min value */
                              G_MAXUINT,  /* max value */
                              0,          /* default value */
                              G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_NDDE_LINE_NUMBER,
                                    pspec);

    /* install default implementation of print method */
    IR_NODE_CLASS(klass)->do_print = ir_node_do_print;
}

static void
ir_node_set_property(GObject *object,
                     guint property_id,
                     const GValue *value,
                     GParamSpec *pspec)
{
    assert(IR_IS_NODE(object));
    /* we only have one property */
    assert(property_id == IR_NDDE_LINE_NUMBER);
    IrNode *node = IR_NODE(object);

    node->line_number = g_value_get_uint(value);
}

static void
ir_node_get_property(GObject *object,
                     guint property_id,
                     GValue *value,
                     GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static void
ir_node_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_NODE(self));
    assert(out);

    fprintf_indent(out, indention, "IrNode %s [%p]\n", 
                   g_type_name(G_TYPE_FROM_INSTANCE(self)), self);
}


