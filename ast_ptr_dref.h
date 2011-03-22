#ifndef AST_PTR_DREF_INC_X
#define AST_PTR_DREF_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_PTR_DREF ast_ptr_dref_get_type()

#define AST_PTR_DREF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_PTR_DREF, AstPtrDref))

#define AST_PTR_DREF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_PTR_DREF, AstPtrDrefClass))

#define AST_IS_PTR_DREF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_PTR_DREF))

#define AST_IS_PTR_DREF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_PTR_DREF))

#define AST_PTR_DREF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_PTR_DREF, AstIdentClass))

typedef struct
{
    AstExpression parent;
    /* private */
    AstExpression *ptr_expression;
} AstPtrDref;

typedef struct
{
    AstExpressionClass parent_class;
} AstPtrDrefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_ptr_dref_get_type(void);

AstPtrDref *
ast_ptr_dref_new(AstExpression *ptr_expression, guint line_number);

AstExpression *
ast_ptr_dref_get_ptr_expression(AstPtrDref *self);

#endif /* AST_PTR_DREF_INC_X */
