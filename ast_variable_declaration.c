#include <string.h>

#include "ast_variable_declaration.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_variable_declaration_do_print(AstNode *self, FILE *out);

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
      type = g_type_register_static(XDP_TYPE_AST_NODE,
                                    "AstVariableDeclarationType",
                                    &info, 0);
    }
    return type;
}

AstVariableDeclaration *
ast_variable_declaration_new(DtDataType *type, char *name)
{
    AstVariableDeclaration *node;

    node = g_object_new(XDP_TYPE_AST_VARIABLE_DECLARATION, NULL);
    node->type = type;
    node->name = strdup(name);

    return node;
}


DtDataType *
ast_variable_declaration_get_data_type(AstVariableDeclaration *self)
{
    assert(self);
    assert(XDP_IS_AST_VARIABLE_DECLARATION(self));

    return self->type;
}

char *
ast_variable_declaration_get_name(AstVariableDeclaration *self)
{
    assert(self);
    assert(XDP_IS_AST_VARIABLE_DECLARATION(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_variable_declaration_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);

    AstVariableDeclaration *var_dec = (AstVariableDeclaration *)self;

    dt_data_type_print(var_dec->type, out);
    fprintf(out, " %s", var_dec->name);
}

static void
ast_variable_declaration_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_variable_declaration_do_print;
}

