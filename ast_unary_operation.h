#ifndef AST_UNARY_OPERATION_INC_X
#define AST_UNARY_OPERATION_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_UNARY_OPERATION ast_unary_operation_get_type()

#define XDP_AST_UNARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_UNARY_OPERATION, AstUnaryOperation))

#define XDP_AST_UNARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_UNARY_OPERATION, AstUnaryOperationClass))

#define XDP_IS_AST_UNARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_UNARY_OPERATION))

#define XDP_IS_AST_UNARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_UNARY_OPERATION))

#define XDP_AST_UNARY_OPERATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_UNARY_OPERATION, AstUnaryOperationClass))

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
    ast_unary_op_type_t  type;
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
ast_unary_operation_new(ast_unary_op_type_t type,
                        AstExpression *operand);

#endif /* AST_UNARY_OPERATION_INC_X */
