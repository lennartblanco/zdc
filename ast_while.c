#include "ast_while.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_while_do_print(AstNode *self, FILE *out);

static void
ast_while_class_init(gpointer klass, gpointer dummy);


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_while_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstWhileClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_while_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstWhile),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_STATMENT,
                                    "AstWhileType",
                                    &info, 0);
    }
    return type;
}

AstWhile * 
ast_while_new(AstExpression *loop_condition,
              AstCodeBlock *body)
{
    assert(XDP_IS_AST_EXPRESSION(loop_condition));

    AstWhile *obj;

    obj = g_object_new(AST_TYPE_WHILE, NULL);
    obj->loop_condition = loop_condition;
    obj->body = body;

    return obj;
}

AstExpression *
ast_while_get_loop_condition(AstWhile *self)
{
    assert(AST_IS_WHILE(self));

    return self->loop_condition;
}

AstCodeBlock *
ast_while_get_body(AstWhile *self)
{
    assert(AST_IS_WHILE(self));

    return self->body;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_while_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);
    assert(AST_IS_WHILE(self));

    AstWhile *while_loop = AST_WHILE(self);

    fprintf(out, "  while ( ");
    ast_node_print(XDP_AST_NODE(while_loop->loop_condition), out);
    fprintf(out, " )\n");
    ast_node_print(XDP_AST_NODE(while_loop->body), out);
}

static void
ast_while_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_while_do_print;
}

