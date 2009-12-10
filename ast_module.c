#include "ast_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_module_do_print(AstNode *self, FILE *out);

static void
ast_module_class_init(gpointer klass, gpointer unused);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_module_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstModuleClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_module_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstModule),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstModuleType",
                                    &info, 0);
    }
    return type;
}

AstModule *
ast_module_new(void)
{
    AstModule *node;

    node = g_object_new(AST_TYPE_MODULE, NULL);
    node->function_defs = NULL;
    node->function_decls = NULL;
    node->package = NULL;

    return node;
}

void
ast_module_set_package(AstModule *self, GSList *package_names)
{
    assert(AST_IS_MODULE(self));

    self->package = package_names;
}

void
ast_module_add_function_decl(AstModule *self,
                             AstFunctionDecl *function_decl)
{
    assert(AST_IS_MODULE(self));
    assert(AST_IS_FUNCTION_DECL(function_decl));

    self->function_decls = g_slist_append(self->function_decls, function_decl);
}

void
ast_module_add_function_def(AstModule *self,
                            AstFunctionDef *function_def)
{
    assert(AST_IS_MODULE(self));
    assert(AST_IS_FUNCTION_DEF(function_def));

    self->function_defs = g_slist_append(self->function_defs, function_def);
}

GSList *
ast_module_get_function_decls(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->function_decls;
}

GSList *
ast_module_get_function_defs(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->function_defs;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_module_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_MODULE(self));
    assert(out);

    AstModule *module = (AstModule *)self;
    GSList *i;

    fprintf(out, "module [%p]\n", module);

    if (module->package != NULL)
    {
        fprintf(out, "  package: ");
        for (i = module->package; i != NULL; i = g_slist_next(i))
        {
            fprintf(out, "%s%s", (char *)i->data,
                    g_slist_next(i) != NULL ? "." : "\n");
        }
    }

    for (i = module->function_defs; i != NULL; i = g_slist_next(i))
    {
        ast_node_print(AST_NODE(i->data), out);
    }

    for (i = module->function_decls; i != NULL; i = g_slist_next(i))
    {
        ast_node_print(AST_NODE(i->data), out);
    }
}

static void
ast_module_class_init(gpointer klass, gpointer unused)
{
    ((AstNodeClass *)klass)->do_print = ast_module_do_print;
}