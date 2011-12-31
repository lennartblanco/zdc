#include <stdbool.h>

#include "ast_binary_operation.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_binary_operation_do_print(AstNode *self, FILE *out, int indention);

static void
ast_binary_operation_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_binary_operation_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstBinaryOperationClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_binary_operation_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstBinaryOperation),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstBinaryOperationType",
                                    &info, 0);
    }
    return type;
}

AstBinaryOperation *
ast_binary_operation_new(binary_op_type_t operation,
                         AstExpression *left,
                         AstExpression *right,
                         guint line_number)
{
    AstBinaryOperation *obj;

    obj = g_object_new(AST_TYPE_BINARY_OPERATION,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->operation = operation;
    obj->left = left;
    obj->right = right;

    return obj;
}

binary_op_type_t
ast_binary_operation_get_operation(AstBinaryOperation *self)
{
    assert(AST_IS_BINARY_OPERATION(self));

    return self->operation;
}

AstExpression *
ast_binary_operation_get_left(AstBinaryOperation *self)
{
    assert(AST_IS_BINARY_OPERATION(self));

    return self->left;
}

AstExpression *
ast_binary_operation_get_right(AstBinaryOperation *self)
{
    assert(AST_IS_BINARY_OPERATION(self));

    return self->right;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_binary_operation_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_BINARY_OPERATION(self));
    assert(out);

    AstBinaryOperation *bin_op = (AstBinaryOperation *)self;

    char *str;
    ast_node_print(AST_NODE(bin_op->left), out, indention);
    switch (bin_op->operation)
    {
        case op_less:           /*  <  */
            str = "<";
            break;
        case op_greater:        /*  >  */
            str = ">";
            break;
        case op_less_or_eq:     /* <=  */
            str = "<=";
            break;
        case op_greater_or_eq:  /* >=  */
            str = ">=";
            break;
        case op_equal:          /* ==  */
            str = "==";
            break;
        case op_not_equal:      /* !=  */
            str = "!=";
            break;
        case op_plus:           /*  +  */
            str = "+";
            break;
        case op_minus:          /*  -  */
            str = "-";
            break;
        case op_mult:           /*  *  */
            str = "*";
            break;
        case op_division:        /*  /  */
            str = "/";
            break;
        case op_or:
            str = "||";
            break;
        case op_and:
            str = "&&";
            break;
        default:
            /* unexpected binary op */
            assert(false);
            break;
    }
    fprintf(out, " %s ", str);
    ast_node_print(AST_NODE(bin_op->right), out, indention);
}

static void
ast_binary_operation_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_binary_operation_do_print;
}


