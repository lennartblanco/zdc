#ifndef AST_BINARY_OPERATION_INC_X
#define AST_BINARY_OPERATION_INC_X

#include "operations.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_BINARY_OPERATION ast_binary_operation_get_type()

#define AST_BINARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_BINARY_OPERATION, AstBinaryOperation))

#define AST_BINARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_BINARY_OPERATION, AstBinaryOperationClass))

#define AST_IS_BINARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_BINARY_OPERATION))

#define AST_IS_BINARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_BINARY_OPERATION))

#define AST_BINARY_OPERATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_BINARY_OPERATION, AstBinaryOperationClass))

typedef struct
{
    AstExpression         parent;

    /* private */
    binary_op_type_t      operation;
    AstExpression         *left;
    AstExpression         *right;
} AstBinaryOperation;

typedef struct
{
    AstExpressionClass parent_class;
} AstBinaryOperationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_binary_operation_get_type(void);

AstBinaryOperation *
ast_binary_operation_new(binary_op_type_t operation,
                         AstExpression *left,
                         AstExpression *right,
                         guint line_number);

binary_op_type_t
ast_binary_operation_get_operation(AstBinaryOperation *self);

/**
 * Retrive left operand expression.
 */
AstExpression *
ast_binary_operation_get_left(AstBinaryOperation *self);

/**
 * Retrive right operand expression.
 */
AstExpression *
ast_binary_operation_get_right(AstBinaryOperation *self);

#endif /* AST_BINARY_OPERATION_INC_X */
