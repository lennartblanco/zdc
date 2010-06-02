#include <string.h>

#include "ast_variable_definition.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_variable_definition_do_print(AstNode *self, FILE *out, int indention);

static void
ast_variable_definition_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_variable_definition_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstVariableDefinitionClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_variable_definition_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstVariableDefinition),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstVariableDefinitionType",
                                    &info, 0);
    }
    return type;
}

AstVariableDefinition *
ast_variable_definition_new(DtDataType *type,
                            char *name, 
                            AstExpression *initializer,
                            guint line_number)
{
    AstVariableDefinition *obj;

    obj = g_object_new(AST_TYPE_VARIABLE_DEFINITION,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->type = type;
    obj->name = strdup(name);
    obj->initializer = initializer;

    return obj;
}

DtDataType *
ast_variable_definition_get_data_type(AstVariableDefinition *self)
{
    assert(AST_IS_VARIABLE_DEFINITION(self));

    return self->type;
}

char *
ast_variable_definition_get_name(AstVariableDefinition *self)
{
    assert(AST_IS_VARIABLE_DEFINITION(self));

    return self->name;
}

AstExpression *
ast_variable_definition_get_initializer(AstVariableDefinition *self)
{
    assert(AST_IS_VARIABLE_DEFINITION(self));

    return self->initializer;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_variable_definition_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_VARIABLE_DEFINITION(self));
    assert(out);

    AstVariableDefinition *obj = (AstVariableDefinition *)self;

    fprintf(out, "  %s %s",
            dt_data_type_get_string(obj->type),
            obj->name);

    if (obj->initializer != NULL)
    {
        fprintf(out, " = ");
        ast_node_print(AST_NODE(obj->initializer), out, indention);
    }
    fprintf(out, "\n");
}

static void
ast_variable_definition_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_variable_definition_do_print;
}

