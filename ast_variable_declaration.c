#include <string.h>

#include "ast_variable_declaration.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_variable_declaration_do_print(AstNode *self, FILE *out, int indention);

static void
ast_variable_declaration_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_variable_declaration_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstVariableDeclarationClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_variable_declaration_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstVariableDeclaration),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstVariableDeclarationType",
                                    &info, 0);
    }
    return type;
}

AstVariableDeclaration *
ast_variable_declaration_new(bool is_ref,
                             DtDataType *type,
                             char *name,
                             guint line_number)
{
    assert(!is_ref); /* reference function parameters not implemented */
    assert(DT_IS_DATA_TYPE(type));

    AstVariableDeclaration *node;

    node = g_object_new(AST_TYPE_VARIABLE_DECLARATION,
                        "ast-node-line-number", line_number,
                        NULL);
    node->type = type;
    node->name = strdup(name);

    return node;
}


DtDataType *
ast_variable_declaration_get_data_type(AstVariableDeclaration *self)
{
    assert(AST_IS_VARIABLE_DECLARATION(self));

    return self->type;
}

char *
ast_variable_declaration_get_name(AstVariableDeclaration *self)
{
    assert(AST_IS_VARIABLE_DECLARATION(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_variable_declaration_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_VARIABLE_DECLARATION(self));
    assert(out);

    AstVariableDeclaration *var_dec = (AstVariableDeclaration *)self;

    fprintf(out, "%s %s",
            dt_data_type_get_string(var_dec->type),
            var_dec->name);
}

static void
ast_variable_declaration_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_variable_declaration_do_print;
}

