#include <string.h>

#include "ast_variable_definition.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_variable_definition_do_print(AstNode *self, FILE *out);

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
      type = g_type_register_static(XDP_TYPE_AST_STATMENT,
                                    "AstVariableDefinitionType",
                                    &info, 0);
    }
    return type;
}

AstVariableDefinition *
ast_variable_definition_new(AstDataType *type,
                            char *name, 
                            AstExpression *initializer)
{
    AstVariableDefinition *obj;

    obj = g_object_new(XDP_TYPE_AST_VARIABLE_DEFINITION, NULL);
    obj->type = type;
    obj->name = strdup(name);
    obj->initializer = initializer;

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_variable_definition_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);

    AstVariableDefinition *obj = (AstVariableDefinition *)self;

    fprintf(out, "  ");
    ast_node_print(XDP_AST_NODE(obj->type), out);
    fprintf(out, " %s", obj->name);
    if (obj->initializer != NULL)
    {
        fprintf(out, " = ");
        ast_node_print(XDP_AST_NODE(obj->initializer), out);
    }
    fprintf(out, "\n");
}

static void
ast_variable_definition_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_variable_definition_do_print;
}

