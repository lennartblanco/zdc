#include "ast_assigment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_assigment_do_print(AstNode *self, FILE *out);

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
ast_assigment_new(AstVariableRef *target,
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

AstVariableRef *
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
ast_assigment_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_ASSIGMENT(self));
    assert(out);

    AstAssigment *obj = (AstAssigment *)self;

    fprintf(out, "  ");
    ast_node_print(AST_NODE(obj->target), out);
    fprintf(out, " = ");
    ast_node_print(AST_NODE(obj->value), out);
    fprintf(out, "\n");
}

static void
ast_assigment_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_assigment_do_print;
}

