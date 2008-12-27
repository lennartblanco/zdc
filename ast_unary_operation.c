#include "ast_unary_operation.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_unary_operation_do_print(AstNode *self, FILE *out);

static void
ast_unary_operation_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_unary_operation_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstUnaryOperationClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_unary_operation_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstUnaryOperation),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_EXPRESSION,
                                    "AstUnaryOperationType",
                                    &info, 0);
    }
    return type;
}

AstUnaryOperation *
ast_unary_operation_new(ast_unary_op_type_t operation,
                        AstExpression *operand)
{
    AstUnaryOperation *obj;

    obj = g_object_new(XDP_TYPE_AST_UNARY_OPERATION, NULL);
    obj->operation = operation;
    obj->operand = operand;

    return obj;
}

ast_unary_op_type_t
ast_unary_operation_get_operation(AstUnaryOperation *self)
{
    assert(self);
    assert(XDP_IS_AST_UNARY_OPERATION(self));

    return self->operation;
}

AstExpression *
ast_unary_operation_get_operand(AstUnaryOperation *self)
{
    assert(self);
    assert(XDP_IS_AST_UNARY_OPERATION(self));

    return self->operand;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_unary_operation_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_UNARY_OPERATION(self));
    assert(out);

    AstUnaryOperation *op = (AstUnaryOperation *)self;

    char *str;
    switch (op->operation)
    {
        case ast_arithm_neg_op:           /*  -  */
            str = "-";
            break;
        case ast_bool_neg_op:             /*  !  */
            str = "!";
            break;
    }
    fprintf(out, "%s(", str);
    ast_node_print(XDP_AST_NODE(op->operand), out);
    fprintf(out, ")");
}

static void
ast_unary_operation_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_unary_operation_do_print;
}

