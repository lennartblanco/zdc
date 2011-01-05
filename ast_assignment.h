#ifndef AST_ASSIGNMENT_INC_X
#define AST_ASSIGNMENT_INC_X

#include "ast_statment.h"
#include "ast_ident.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ASSIGNMENT ast_assignment_get_type()

#define AST_ASSIGNMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ASSIGNMENT, AstAssignment))

#define AST_ASSIGNMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ASSIGNMENT, AstAssignmentClass))

#define AST_IS_ASSIGNMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ASSIGNMENT))

#define AST_IS_ASSIGNMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ASSIGNMENT))

#define AST_ASSIGNMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ASSIGNMENT, AstAssignmentClass))

typedef struct
{
    AstStatment parent;
    /* private */
    AstIdent *target;
    AstExpression  *value;
} AstAssignment;

typedef struct
{
    AstStatmentClass parent_class;
} AstAssignmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_assignment_get_type(void);

AstAssignment *
ast_assignment_new(AstIdent *target,
                   AstExpression *value,
                   guint line_number);

AstIdent *
ast_assignment_get_target(AstAssignment *self);

AstExpression *
ast_assignment_get_value(AstAssignment *self);

#endif /* AST_ASSIGNMENT_INC_X */
