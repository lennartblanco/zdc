#ifndef AST_VARIABLE_DEFINITION_INC_X
#define AST_VARIABLE_DEFINITION_INC_X

#include "ast_statment.h"
#include "ast_expression.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_VARIABLE_DEFINITION ast_variable_definition_get_type()

#define AST_VARIABLE_DEFINITION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_VARIABLE_DEFINITION, AstVariableDefinition))

#define AST_VARIABLE_DEFINITION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_VARIABLE_DEFINITION, AstVariableDefinitionClass))

#define AST_IS_VARIABLE_DEFINITION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_VARIABLE_DEFINITION))

#define AST_IS_VARIABLE_DEFINITION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_VARIABLE_DEFINITION))

#define AST_VARIABLE_DEFINITION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_VARIABLE_DEFINITION, AstVariableDefinitionClass))

typedef struct
{
    AstStatment      parent;

    /* private */
    DtDataType    *type;
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
ast_variable_definition_new(DtDataType *type,
                            char *name, 
                            AstExpression *initializer,
                            guint line_number);

DtDataType *
ast_variable_definition_get_data_type(AstVariableDefinition *self);

char *
ast_variable_definition_get_name(AstVariableDefinition *self);

AstExpression *
ast_variable_definition_get_initializer(AstVariableDefinition *self);

#endif /* AST_VARIABLE_DEFINITION_INC_X */
