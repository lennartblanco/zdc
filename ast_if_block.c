#include "ast_if_block.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_if_block_do_print(AstNode *self, FILE *out, int indention);

static void
ast_if_block_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_if_block_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstIfBlockClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_if_block_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstIfBlock),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstIfBlockType",
                                    &info, 0);
    }
    return type;
}

AstIfBlock *
ast_if_block_new(AstExpression *condition, AstCodeBlock *body)
{
    AstIfBlock *obj;

    obj = g_object_new(AST_TYPE_IF_BLOCK, NULL);
    obj->condition = condition;
    obj->body = body;

    return obj;
}

AstExpression *
ast_if_block_get_condition(AstIfBlock *self)
{
    assert(AST_IS_IF_BLOCK(self));

    return self->condition;
}

AstCodeBlock *
ast_if_block_get_body(AstIfBlock *self)
{
    assert(AST_IS_IF_BLOCK(self));

    return self->body;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_if_block_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_IF_BLOCK(self));
    assert(out);

    AstIfBlock *if_block = (AstIfBlock *)self;

    fprintf(out, "   if (");

    ast_node_print(AST_NODE(if_block->condition), out, indention);

    fprintf(out, ")\n");
    ast_node_print(AST_NODE(if_block->body), out, indention);
}

static void
ast_if_block_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_if_block_do_print;
}

