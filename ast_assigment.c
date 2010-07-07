#include "ast_assigment.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_assigment_do_print(AstNode *self, FILE *out, int indention);

static void
ast_assigment_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_assigment_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstAssigmentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_assigment_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAssigment),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstAssigmentType",
                                    &info, 0);
    }
    return type;
}

AstAssigment *
ast_assigment_new(AstIdent *target,
                  AstExpression *value,
                  guint line_number)
{
    AstAssigment *obj;

    obj = g_object_new(AST_TYPE_ASSIGMENT,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->target = target;
    obj->value = value;

    return obj;
}

AstIdent *
ast_assigment_get_target(AstAssigment *self)
{
    assert(AST_IS_ASSIGMENT(self));

    return self->target;
}

AstExpression *
ast_assigment_get_value(AstAssigment *self)
{
    assert(AST_IS_ASSIGMENT(self));

    return self->value;
}



/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_assigment_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_ASSIGMENT(self));
    assert(out);

    AstAssigment *obj = (AstAssigment *)self;
    fprintf_indent(out, indention, "assigment\n  target:\n");
    ast_node_print(AST_NODE(obj->target), out, indention + 4);
    fprintf_indent(out, indention, "\n  value:\n");
    ast_node_print(AST_NODE(obj->value), out, indention + 4);
}

static void
ast_assigment_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_assigment_do_print;
}

