#ifndef AST_ASSIGMENT_INC_X
#define AST_ASSIGMENT_INC_X

#include "ast_statment.h"
#include "ast_variable_ref.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_ASSIGMENT ast_assigment_get_type()

#define XDP_AST_ASSIGMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_ASSIGMENT, AstAssigment))

#define XDP_AST_ASSIGMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_ASSIGMENT, AstAssigmentClass))

#define XDP_IS_AST_ASSIGMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_ASSIGMENT))

#define XDP_IS_AST_ASSIGMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_ASSIGMENT))

#define XDP_AST_ASSIGMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_ASSIGMENT, AstAssigmentClass))

typedef struct
{
    AstStatment parent;
    /* private */
    AstVariableRef *target;
    AstExpression  *value;
} AstAssigment;

typedef struct
{
    AstStatmentClass parent_class;
} AstAssigmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_assigment_get_type(void);

AstAssigment *
ast_assigment_new(AstVariableRef *target,
                  AstExpression *value,
                  guint line_number);

AstVariableRef *
ast_assigment_get_target(AstAssigment *self);

AstExpression *
ast_assigment_get_value(AstAssigment *self);

#endif /* AST_ASSIGMENT_INC_X */
