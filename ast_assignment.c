#include "ast_assignment.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_assignment_do_print(AstNode *self, FILE *out, int indention);

static void
ast_assignment_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_assignment_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstAssignmentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_assignment_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAssignment),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstAssignmentType",
                                    &info, 0);
    }
    return type;
}

AstAssignment *
ast_assignment_new(AstExpression *lvalue,
                   AstExpression *value,
                   guint line_number)
{
    AstAssignment *obj;

    obj = g_object_new(AST_TYPE_ASSIGNMENT,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->lvalue = lvalue;
    obj->value = value;

    return obj;
}

AstExpression *
ast_assignment_get_lvalue(AstAssignment *self)
{
    assert(AST_IS_ASSIGNMENT(self));

    return self->lvalue;
}

AstExpression *
ast_assignment_get_value(AstAssignment *self)
{
    assert(AST_IS_ASSIGNMENT(self));

    return self->value;
}



/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_assignment_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_ASSIGNMENT(self));
    assert(out);

    AstAssignment *obj = (AstAssignment *)self;
    fprintf_indent(out, indention, "assignment\n  lvalue:\n");
    ast_node_print(AST_NODE(obj->lvalue), out, indention + 4);
    fprintf_indent(out, indention, "\n  value:\n");
    ast_node_print(AST_NODE(obj->value), out, indention + 4);
}

static void
ast_assignment_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_assignment_do_print;
}

