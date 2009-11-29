#include "ast_if_else.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_if_else_do_print(AstNode *self, FILE *out);

static void
ast_if_else_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_if_else_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstIfElseClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_if_else_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstIfElse),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstIfElseType",
                                    &info, 0);
    }
    return type;
}

AstIfElse* 
ast_if_else_new()
{
    AstIfElse *obj;

    obj = g_object_new(AST_TYPE_IF_ELSE, NULL);
    obj->if_else_blocks = NULL;
    obj->else_block = NULL;

    return obj;
}

void
ast_if_else_add_if_else_block(AstIfElse *self, AstIfBlock *if_else_block)
{
    assert(AST_IS_IF_ELSE(self));
    assert(AST_IS_IF_BLOCK(if_else_block));

    self->if_else_blocks = g_slist_append(self->if_else_blocks, if_else_block);
}

void
ast_if_else_set_else_block(AstIfElse *self, AstCodeBlock *else_block)
{
    assert(AST_IS_IF_ELSE(self));
    assert(AST_IS_CODE_BLOCK(else_block));

    self->else_block = else_block;
}

GSList *
ast_if_else_get_if_else_blocks(AstIfElse *self)
{
    assert(AST_IS_IF_ELSE(self));

    return self->if_else_blocks;
}

AstCodeBlock *
ast_if_else_get_else_block(AstIfElse *self)
{
    assert(AST_IS_IF_ELSE(self));

    return self->else_block;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_if_else_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_IF_ELSE(self));
    assert(out);

    AstIfElse *ifelse = AST_IF_ELSE(self);
    fprintf(out, "  if-else [%p]\n  (:\n", self);

    GSList *p = ifelse->if_else_blocks;
    for (;p != NULL; p = g_slist_next(p))
    {
        ast_node_print(AST_NODE(p->data), out);
    }

    if (ifelse->else_block != NULL)
    {
        ast_node_print(AST_NODE(ifelse->else_block), out);
    }

    fprintf(out, "  :)\n");
}

static void
ast_if_else_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_if_else_do_print;
}

