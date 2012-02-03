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
    GSList *declarations;
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_module_do_print(AstNode *self, FILE *out, int indention);

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
    node->package = NULL;
    node->imports = NULL;
    node->declarations = NULL;

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

const char *
ast_module_get_source_file(AstModule *self)
{
    assert(AST_IS_MODULE(self));
    assert(self->source_file);

    return self->source_file;
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
ast_module_add_imports(AstModule *self, GSList *imports)
{
    assert(AST_IS_MODULE(self));

    self->imports = g_slist_concat(self->imports, imports);
}

GSList *
ast_module_get_imports(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->imports;
}

void
ast_module_add_declaration(AstModule *self, AstDeclaration *decl)
{
    assert(AST_IS_MODULE(self));
    assert(AST_IS_DECLARATION(decl));

    self->declarations = g_slist_append(self->declarations, decl);
}

GSList *
ast_module_get_declarations(AstModule *self)
{
    assert(AST_IS_MODULE(self));

    return self->declarations;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_module_do_print(AstNode *self, FILE *out, int indention)
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
            ast_node_print(i->data, out, indention);
        }
    }
}

static void
ast_module_class_init(gpointer klass, gpointer unused)
{
    ((AstNodeClass *)klass)->do_print = ast_module_do_print;
}
