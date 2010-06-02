#include <stdbool.h>

#include "ast_node.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    AST_NDDE_LINE_NUMBER = 1
};


/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_node_do_print(AstNode *self, FILE *out, int indention);

static void
ast_node_class_init(gpointer klass, gpointer foo);

static void
ast_node_set_property(GObject *object,
                      guint property_id,
                      const GValue *value,
                      GParamSpec *pspec);

static void
ast_node_get_property(GObject *object,
                      guint property_id,
                      GValue *value,
                      GParamSpec *pspec);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_node_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstNodeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_node_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstNode),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "AstNodeType",
                                    &info, 0);
    }
    return type;
}

guint
ast_node_get_line_num(void *self)
{
    assert(AST_IS_NODE(self));

    return (AST_NODE(self)->line_number);
}

void
ast_node_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_NODE(self));

    AST_NODE_GET_CLASS(self)->do_print(self, out, indention);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/


static void
ast_node_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_NODE(self));

    fprintf(out, "AST node %s [%p]\n",
            g_type_name(G_TYPE_FROM_INSTANCE(self)), self);
}

static void
ast_node_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ast_node_set_property;
    gobject_class->get_property = ast_node_get_property;

    /*
     * install 'line-number' property 
     */
    pspec = g_param_spec_uint("ast-node-line-number",
                              "ast node line number",
                              "source file line number",
                              0,          /* min value */
                              G_MAXUINT,  /* max value */
                              0,          /* default value */
                              G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    AST_NDDE_LINE_NUMBER,
                                    pspec);


    /* install default implementation of print method */
    ((AstNodeClass *)klass)->do_print = ast_node_do_print;
}

static void
ast_node_set_property(GObject *object,
                      guint property_id,
                      const GValue *value,
                      GParamSpec *pspec)
{
    assert(AST_IS_NODE(object));

    /* we only have one property */
    assert(property_id == AST_NDDE_LINE_NUMBER);
    AstNode *node = AST_NODE(object);

    node->line_number = g_value_get_uint(value);
}

static void
ast_node_get_property(GObject *object,
                      guint property_id,
                      GValue *value,
                      GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

