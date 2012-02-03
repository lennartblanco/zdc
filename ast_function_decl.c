#include <string.h>

#include "ast_function_decl.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_function_decl_do_print(AstNode *self, FILE *out, int indention);

static void
ast_function_decl_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_decl_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstFunctionDeclClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_function_decl_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstFunctionDecl),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_DECLARATION,
                                    "AstFunctionDeclType",
                                    &info, 0);
    }
    return type;
}

AstFunctionDecl *
ast_function_decl_new(AstAttributes *attrs,
                      DtDataType *return_type,
                      char *name,
                      GSList *parameters,
                      guint line_number)
{
    AstFunctionDecl *func;

    func = g_object_new(AST_TYPE_FUNCTION_DECL,
                        "ast-node-line-number", line_number,
                        NULL);

    func->attrs = attrs;
    func->return_type = return_type;
    func->name = strdup(name);
    func->parameters = parameters;

    return func;
}

AstAttributes *
ast_function_decl_get_attributes(AstFunctionDecl *self)
{
    assert(AST_IS_FUNCTION_DECL(self));

    return self->attrs;
}


char *
ast_function_decl_get_name(AstFunctionDecl *self)
{
    assert(AST_IS_FUNCTION_DECL(self));

    return self->name;
}

GSList *
ast_function_decl_get_parameters(AstFunctionDecl *self)
{
    assert(AST_IS_FUNCTION_DECL(self));

    return self->parameters;
}

DtDataType *
ast_function_decl_get_return_type(AstFunctionDecl *self)
{
    assert(AST_IS_FUNCTION_DECL(self));

    return self->return_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_function_decl_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_FUNCTION_DECL(self));
    assert(out);

    AstFunctionDecl *func = (AstFunctionDecl *)self;
    fprintf(out, "function declaration [%p]\n" \
                 "  name: %s return type: %s\n", 
                 self, func->name,
                 dt_data_type_get_string(func->return_type));

    fprintf(out, "  params: ");

    GSList *p = func->parameters;
    while(p != NULL)
    {
        ast_node_print(AST_NODE(p->data), out, indention);
        if (p->next != NULL)
        {
            fprintf(out, ", ");
        }
        p = p->next;
    }
    fprintf(out, "\n");
}

static void
ast_function_decl_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_function_decl_do_print;
}

