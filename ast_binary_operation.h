#ifndef AST_BINARY_OPERATION_INC_X
#define AST_BINARY_OPERATION_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_BINARY_OPERATION ast_binary_operation_get_type()

#define XDP_AST_BINARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_BINARY_OPERATION, AstBinaryOperation))

#define XDP_AST_BINARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_BINARY_OPERATION, AstBinaryOperationClass))

#define XDP_IS_AST_BINARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_BINARY_OPERATION))

#define XDP_IS_AST_BINARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_BINARY_OPERATION))

#define XDP_AST_BINARY_OPERATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_BINARY_OPERATION, AstBinaryOperationClass))

/*
 * binary operations types
 */
typedef enum ast_binary_op_type_e
{
    ast_or_op,             /* || */
    ast_and_op,            /* && */
    ast_less_op,           /*  <  */
    ast_greater_op,        /*  >  */
    ast_less_or_eq_op,     /* <=  */
    ast_greater_or_eq_op,  /* >=  */
    ast_equal_op,          /* ==  */
    ast_not_equal_op,      /* !=  */
    ast_plus_op,           /*  +  */
    ast_minus_op,          /*  -  */
    ast_mult_op,           /*  *  */
    ast_division_op        /*  /  */
} ast_binary_op_type_t;


typedef struct
{
    AstExpression         parent;

    /* private */
    ast_binary_op_type_t  operation;
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
ast_binary_operation_new(ast_binary_op_type_t operation,
                         AstExpression *left,
                         AstExpression *right);

ast_binary_op_type_t
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
