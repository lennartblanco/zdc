#include "ast_code_block.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_code_block_do_print(AstNode *self, FILE *out, int indention);

static void
ast_code_block_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_code_block_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstCodeBlockClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_code_block_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstCodeBlock),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstCodeBlockType",
                                    &info, 0);
    }
    return type;
}

AstCodeBlock *
ast_code_block_new()
{
    AstCodeBlock *code_block;

    code_block = g_object_new(AST_TYPE_CODE_BLOCK, NULL);
    code_block->statments = NULL;

    return code_block;
}

void
ast_code_block_add_statment(AstCodeBlock *self, AstStatment *statment)
{
    assert(AST_IS_CODE_BLOCK(self));
    assert(AST_IS_STATMENT(statment));

    self->statments = g_slist_append(self->statments, statment);
}

GSList *
ast_code_block_get_statments(AstCodeBlock *self)
{
    assert(AST_IS_CODE_BLOCK(self));

    return self->statments;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_code_block_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_CODE_BLOCK(self));
    assert(out);

    GSList *p = ((AstCodeBlock *)self)->statments;
    fprintf(out, "{\n");
    while (p != NULL)
    {
        ast_node_print(AST_NODE(p->data), out, indention + 2);
        p = p->next;
    }
    fprintf(out, "}\n");
}

static void
ast_code_block_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_code_block_do_print;
}

