#include <string.h>

#include "ast_function_decl.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_function_decl_do_print(AstNode *self, FILE *out);

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
      type = g_type_register_static(XDP_TYPE_AST_NODE,
                                    "AstFunctionDeclType",
                                    &info, 0);
    }
    return type;
}

AstFunctionDecl *
ast_function_decl_new(char *name,
                      GSList *parameters,
                      AstDataType *return_type)
{
    AstFunctionDecl *func;

    func = g_object_new(XDP_TYPE_AST_FUNCTION_DECL, NULL);
    func->name = strdup(name);
    func->parameters = parameters;
    func->return_type = return_type;
    func->linkage = NULL;

    return func;
}

void
ast_function_decl_set_linkage(AstFunctionDecl *self,
                              char *linkage)
{
    assert(XDP_IS_AST_FUNCTION_DECL(self));

    self->linkage = linkage;
}

char *
ast_function_decl_get_linkage(AstFunctionDecl *self)
{
    assert(XDP_IS_AST_FUNCTION_DECL(self));

    return self->linkage;
}

char *
ast_function_decl_get_name(AstFunctionDecl *self)
{
    assert(XDP_IS_AST_FUNCTION_DECL(self));

    return self->name;
}

GSList *
ast_function_decl_get_parameters(AstFunctionDecl *self)
{
    assert(XDP_IS_AST_FUNCTION_DECL(self));

    return self->parameters;
}

AstDataType *
ast_function_decl_get_return_type(AstFunctionDecl *self)
{
    assert(XDP_IS_AST_FUNCTION_DECL(self));

    return self->return_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_function_decl_do_print(AstNode *self, FILE *out)
{
    assert(XDP_IS_AST_FUNCTION_DECL(self));
    assert(out);

    AstFunctionDecl *func = (AstFunctionDecl *)self;
    fprintf(out, "function declaration [%p]\n" \
                 "  name: %s return type: ",
                 self, func->name);

    ast_node_print(XDP_AST_NODE(func->return_type), out);

    fprintf(out, "\n  params: ");

    GSList *p = func->parameters;
    while(p != NULL)
    {
        ast_node_print(XDP_AST_NODE(p->data), out);
        if (p->next != NULL)
        {
            fprintf(out, ", ");
        }
        p = p->next;
    }
    fprintf(out, "\n");

    fprintf(out, "  linkage: %s\n",
            func->linkage == NULL ? "default (D)" : func->linkage);
}

static void
ast_function_decl_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_function_decl_do_print;
}

