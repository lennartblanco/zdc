#ifndef AST_ASSIGMENT_INC_X
#define AST_ASSIGMENT_INC_X

#include "ast_statment.h"
#include "ast_ident.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ASSIGMENT ast_assigment_get_type()

#define AST_ASSIGMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ASSIGMENT, AstAssigment))

#define AST_ASSIGMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ASSIGMENT, AstAssigmentClass))

#define AST_IS_ASSIGMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ASSIGMENT))

#define AST_IS_ASSIGMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ASSIGMENT))

#define AST_ASSIGMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ASSIGMENT, AstAssigmentClass))

typedef struct
{
    AstStatment parent;
    /* private */
    AstIdent *target;
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
ast_assigment_new(AstIdent *target,
                  AstExpression *value,
                  guint line_number);

AstIdent *
ast_assigment_get_target(AstAssigment *self);

AstExpression *
ast_assigment_get_value(AstAssigment *self);

#endif /* AST_ASSIGMENT_INC_X */
