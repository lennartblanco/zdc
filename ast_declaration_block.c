#include "ast_declaration_block.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_declaration_block_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstDeclarationBlockClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstDeclarationBlock),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_DECLARATION,
                                    "AstDeclarationBlockType",
                                    &info, 0);
    }
    return type;
}

AstDeclarationBlock *
ast_declaration_block_new(AstAttributes *attrs)
{
    assert(AST_IS_ATTRIBUTES(attrs));

    AstDeclarationBlock *obj = g_object_new(AST_TYPE_DECLARATION_BLOCK, NULL);

    obj->attrs = attrs;
    obj->decls = NULL;

    return obj;
}

void
ast_declaration_block_add_declaration(AstDeclarationBlock *self,
                                      AstDeclaration *decl)
{
    assert(AST_IS_DECLARATION_BLOCK(self));
    assert(AST_IS_DECLARATION(decl));

    self->decls = g_slist_append(self->decls, decl);
}

AstAttributes *
ast_declaration_block_get_attributes(AstDeclarationBlock *self)
{
    assert(AST_IS_DECLARATION_BLOCK(self));

    return self->attrs;
}

GSList *
ast_declaration_block_get_declarations(AstDeclarationBlock *self)
{
    assert(AST_IS_DECLARATION_BLOCK(self));

    return self->decls;
}
