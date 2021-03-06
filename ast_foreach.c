#include "ast_foreach.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_foreach_do_print(AstNode *self, FILE *out, int indention);

static void
ast_foreach_class_init(gpointer klass, gpointer dummy);


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_foreach_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstForeachClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_foreach_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstForeach),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstForeachType",
                                    &info, 0);
    }
    return type;
}

AstForeach *
ast_foreach_new(AstVariableDeclaration *index,
                AstVariableDeclaration *value,
                AstExpression *aggregate,
                AstCodeBlock *body,
                guint line_number)
{
    assert(AST_IS_VARIABLE_DECLARATION(value));
    assert(AST_IS_EXPRESSION(aggregate));

    AstForeach *obj;

    obj = g_object_new(AST_TYPE_FOREACH,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->index = index;
    obj->value = value;
    obj->aggregate = aggregate;
    obj->body = body;

    return obj;
}

AstVariableDeclaration *
ast_foreach_get_index(AstForeach *self)
{
    assert(AST_IS_FOREACH(self));

    return self->index;
}

AstVariableDeclaration *
ast_foreach_get_value(AstForeach *self)
{
    assert(AST_IS_FOREACH(self));

    return self->value;
}

AstExpression *
ast_foreach_get_aggregate(AstForeach *self)
{
    assert(AST_IS_FOREACH(self));

    return self->aggregate;
}

AstCodeBlock *
ast_foreach_get_body(AstForeach *self)
{
    assert(AST_IS_FOREACH(self));

    return self->body;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_foreach_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_FOREACH(self));
    assert(out);

    AstForeach *foreach = AST_FOREACH(self);

    fprintf(out, "  foreach (");
    if (foreach->index != NULL)
    {
        ast_node_print(AST_NODE(foreach->index), out, indention);
        fprintf(out, ", ");
    }
    ast_node_print(AST_NODE(foreach->value), out, indention);
    fprintf(out, "; ");

    ast_node_print(AST_NODE(foreach->aggregate), out, indention);
    fprintf(out, ")\n");
    ast_node_print(AST_NODE(foreach->body), out, indention);
    fprintf(out, "\n");
}

static void
ast_foreach_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_foreach_do_print;
}

