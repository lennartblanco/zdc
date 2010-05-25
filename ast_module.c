#include <string.h>

#include "ast_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct _AstModule
{
    AstNode parent;

    /* private */
    char *source_file;
    GSList *package;
    GSList *imports;
    GSList *function_defs;
    GSList *function_decls;
    GSList *enums;
    GSList *alias;
};

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

    node = g_object_new	(AST_TYPE_MODULE, NULL);
    node->source_file = NULL;
    node->function_defs = NULL;
    node->function_decls = NULL;
    node->package = NULL;
    node->imports = NULL;
    node->enums = NULL;
    node->alias = NULL;

    return node;
}

void
ast_module_set_source_file(AstModule *self, const char *source_file)
{
    assert(AST_IS_MODULE(self));
    assert(self->source_file == NULL);
    assert(source_file);

    self->source_file = strdup(source_file);
}

void
ast_module_set_package(AstModule *self, GSList *package_names)
{
    assert(AST_IS_MODULE(self));

    self->package = package_names;
}

GSList *
ast_module_get_package(AstModule *self)
{
    assert(AST_IS_MODULE(self));
    if (self->package == NULL)
    {
        char *module_name;
        /*
         * module name not explicitly specified,
         * generate it from source file name
         */
        module_name = g_strndup(self->source_file,
                                /* 'file_name' - '.d' */
                                strlen(self->source_file) - 2);
        self->package = g_slist_append(self->package, module_name);
    }

    return self->package;
}

void
ast_module_add_import(AstModule *self, AstImport *imported_module)
{
    assert(AST_IS_MODULE(self));
    assert(AST_IS_IMPORT(imported_module));

    self->imports = g_slist_prepend(self->imports, imported_module);
}

GSList *
ast_module_get_imports(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->imports;
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

void
ast_module_add_enum_def(AstModule *self, AstEnum *enum_def)
{
    assert(AST_IS_MODULE(self));
    assert(AST_IS_ENUM(enum_def));

    self->enums = g_slist_prepend(self->enums, enum_def);
}

GSList *
ast_module_get_enum_defs(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->enums;
}

void
ast_module_add_alias(AstModule *self,
                     AstAlias *alias)
{
    assert(AST_IS_MODULE(self));
    assert(AST_IS_ALIAS(alias));

    self->alias = g_slist_append(self->alias, alias);
}

GSList *
ast_module_get_aliases(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->alias;
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

    fprintf(out, "module [%p]\n  source file: '%s'\n", 
            module, module->source_file);

    if (module->package != NULL)
    {
        fprintf(out, "  package: ");
        for (i = module->package; i != NULL; i = g_slist_next(i))
        {
            fprintf(out, "%s%s", (char *)i->data,
                    g_slist_next(i) != NULL ? "." : "\n");
        }
    }

    if (module->imports != NULL)
    {
        for (i = module->imports; i != NULL; i = g_slist_next(i))
        {
            ast_node_print(i->data, out);
        }
    }

    if (module->alias != NULL)
    {
        for (i = module->alias; i != NULL; i = g_slist_next(i))
        {
            ast_node_print(i->data, out);
        }
    }

    if (module->enums != NULL)
    {
        fprintf(out, "enum definitions:\n");
        for (i = module->enums; i != NULL; i = g_slist_next(i))
        {
            ast_node_print(i->data, out);
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
