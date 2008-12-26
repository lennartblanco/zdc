#include "ast_binary_operation.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_binary_operation_do_print(AstNode *self, FILE *out);

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
      type = g_type_register_static(XDP_TYPE_AST_EXPRESSION,
                                    "AstBinaryOperationType",
                                    &info, 0);
    }
    return type;
}

AstBinaryOperation *
ast_binary_operation_new(ast_binary_op_type_t type,
                         AstExpression *left,
                         AstExpression *right)
{
    AstBinaryOperation *obj;

    obj = g_object_new(XDP_TYPE_AST_BINARY_OPERATION, NULL);
    obj->type = type;
    obj->left = left;
    obj->right = right;

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_binary_operation_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);

    AstBinaryOperation *bin_op = (AstBinaryOperation *)self;

    char *str;
    ast_node_print(XDP_AST_NODE(bin_op->left), out);
    switch (bin_op->type)
    {
        case ast_less_op:           /*  <  */
            str = "<";
            break;
        case ast_greater_op:        /*  >  */
            str = ">";
            break;
        case ast_less_or_eq_op:     /* <=  */
            str = "<=";
            break;
        case ast_greater_or_eq_op:  /* >=  */
            str = ">=";
            break;
        case ast_equal_op:          /* ==  */
            str = "==";
            break;
        case ast_not_equal_op:      /* !=  */
            str = "!=";
            break;
        case ast_plus_op:           /*  +  */
            str = "+";
            break;
        case ast_minus_op:          /*  -  */
            str = "-";
            break;
        case ast_mult_op:           /*  *  */
            str = "*";
            break;
        case ast_division_op:        /*  /  */
            str = "/";
            break;    
    }
    fprintf(out, " %s ", str);
    ast_node_print(XDP_AST_NODE(bin_op->right), out);
}

static void
ast_binary_operation_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_binary_operation_do_print;
}


