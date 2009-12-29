#include "ast_import.h"
#include "ast_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct _AstImport
{
    AstNode    parent;

    /* private */
    GSList  *module_name;
    /** the module ast that is imported */
    AstModule *module;
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_import_do_print(AstNode *self, FILE *out);

static void
ast_import_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_import_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstImportClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_import_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstImport),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstImportType",
                                    &info, 0);
    }
    return type;
}

AstImport *
ast_import_new(GSList *module_name, guint line_number)
{
    assert(g_slist_length(module_name) >= 1);

    AstImport *obj;

    obj = g_object_new(AST_TYPE_IMPORT,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->module_name = module_name;

    return obj;

}

char *
ast_import_get_path(AstImport *self)
{
    assert(AST_IS_IMPORT(self));

    GString *path;
    GSList *i;

    path = g_string_new(NULL);

    for (i = self->module_name; i != NULL; i = g_slist_next(i))
    {
        g_string_append_printf(path, "%s%s",
                               (char*) i->data,
                               g_slist_next(i) != NULL ? "/" : ".d");
    }

    return g_string_free(path, FALSE);
}

void
ast_import_set_module(AstImport *self, AstModule *parsed_module)
{
    assert(AST_IS_IMPORT(self));
    assert(AST_IS_MODULE(parsed_module));

    self->module = parsed_module;
}

AstModule *
ast_import_get_module(AstImport *self)
{
    assert(AST_IS_IMPORT(self));
    assert(AST_IS_MODULE(self->module));

    return self->module;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_import_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_IMPORT(self));
    assert(out);

    AstImport *import = AST_IMPORT(self);
    GSList *i;

    assert(g_slist_length(import->module_name) >= 1);

    fprintf(out, "  import: ");
    for (i = import->module_name; i != NULL; i = g_slist_next(i))
    {
        fprintf(out, "%s%s", (char*)i->data,
                g_slist_next(i) != NULL ? "." : "\n");
    }

}

static void
ast_import_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_import_do_print;
}

