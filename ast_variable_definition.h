#ifndef AST_VARIABLE_DEFINITION_INC_X
#define AST_VARIABLE_DEFINITION_INC_X

#include "ast_statment.h"
#include "ast_data_type.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_VARIABLE_DEFINITION ast_variable_definition_get_type()

#define XDP_AST_VARIABLE_DEFINITION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_VARIABLE_DEFINITION, AstVariableDefinition))

#define XDP_AST_VARIABLE_DEFINITION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_VARIABLE_DEFINITION, AstVariableDefinitionClass))

#define XDP_IS_AST_VARIABLE_DEFINITION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_VARIABLE_DEFINITION))

#define XDP_IS_AST_VARIABLE_DEFINITION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_VARIABLE_DEFINITION))

#define XDP_AST_VARIABLE_DEFINITION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_VARIABLE_DEFINITION, AstVariableDefinitionClass))

typedef struct
{
    AstStatment      parent;

    /* private */
    AstDataType   *type;
    char          *name;
    AstExpression *initializer;
} AstVariableDefinition;

typedef struct
{
  AstStatmentClass parent_class;
} AstVariableDefinitionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_variable_definition_get_type(void);

AstVariableDefinition *
ast_variable_definition_new(AstDataType *type,
                            char *name, 
                            AstExpression *initializer);

AstDataType *
ast_variable_definition_get_data_type(AstVariableDefinition *self);

char *
ast_variable_definition_get_name(AstVariableDefinition *self);

AstExpression *
ast_variable_definition_get_initializer(AstVariableDefinition *self);

#endif /* AST_VARIABLE_DEFINITION_INC_X */
