#include "ast_node.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_node_do_print(AstNode *self, FILE *out);

static void
ast_node_class_init(gpointer klass, gpointer foo);

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

AstNode*
ast_node_new(void)
{
    AstNode *obj;

    obj = g_object_new(XDP_TYPE_AST_NODE, NULL);

    return obj;
}

void
ast_node_print(AstNode *self, FILE *out)
{
    assert(XDP_IS_AST_NODE(self));
    assert(out);

    XDP_AST_NODE_GET_CLASS(self)->do_print(self, out);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/


static void
ast_node_do_print(AstNode *self, FILE *out)
{
    fprintf(out, "Some AST node [%p]\n", self);
}

static void
ast_node_class_init(gpointer klass, gpointer foo)
{
    ((AstNodeClass *)klass)->do_print = ast_node_do_print;
}

