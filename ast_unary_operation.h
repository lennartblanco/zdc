#ifndef AST_UNARY_OPERATION_INC_X
#define AST_UNARY_OPERATION_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_UNARY_OPERATION ast_unary_operation_get_type()

#define AST_UNARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_UNARY_OPERATION, AstUnaryOperation))

#define AST_UNARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_UNARY_OPERATION, AstUnaryOperationClass))

#define AST_IS_UNARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_UNARY_OPERATION))

#define AST_IS_UNARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_UNARY_OPERATION))

#define AST_UNARY_OPERATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_UNARY_OPERATION, AstUnaryOperationClass))

/*
 * binary operations types
 */
typedef enum ast_unary_op_type_e
{
    ast_arithm_neg_op,     /*  -(exp)  */
    ast_bool_neg_op        /*  !(exp)  */
} ast_unary_op_type_t;

typedef struct
{
    AstExpression         parent;

    /* private */
    ast_unary_op_type_t  operation;
    AstExpression        *operand;
} AstUnaryOperation;

typedef struct
{
    AstExpressionClass parent_class;
} AstUnaryOperationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_unary_operation_get_type(void);

AstUnaryOperation *
ast_unary_operation_new(ast_unary_op_type_t operation,
                        AstExpression *operand);

ast_unary_op_type_t
ast_unary_operation_get_operation(AstUnaryOperation *self);

AstExpression *
ast_unary_operation_get_operand(AstUnaryOperation *self);

#endif /* AST_UNARY_OPERATION_INC_X */
