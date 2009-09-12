#include "ast_array_literal.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_array_literal_do_print(AstNode *self, FILE *out);

static void
ast_array_literal_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_array_literal_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstArrayLiteralClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_array_literal_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstArrayLiteral),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_CONSTANT,
                                    "AstArrayConstantType",
                                    &info, 0);
    }
    return type;
}

AstArrayLiteral *
ast_array_literal_new(gint32 value)
{
    AstArrayLiteral *obj;

    obj = g_object_new(XDP_TYPE_AST_ARRAY_LITERAL, NULL);
    obj->values = NULL;

    return obj;
}

void
ast_array_literal_add_value(AstArrayLiteral *self, AstExpression *value)
{
    assert(XDP_IS_AST_ARRAY_LITERAL(self));
    assert(XDP_IS_AST_EXPRESSION(value));

    self->values = g_slist_append(self->values, value);
}

GSList *
ast_array_literal_get_values(AstArrayLiteral *self)
{
    assert(XDP_IS_AST_ARRAY_LITERAL(self));

    return self->values;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_array_literal_do_print(AstNode *self, FILE *out)
{
    assert(XDP_IS_AST_ARRAY_LITERAL(self));
    assert(out);

    fprintf(out, "[");
    GSList *p = XDP_AST_ARRAY_LITERAL(self)->values;
    for (; p != NULL; p = g_slist_next(p))
    {
        ast_node_print(XDP_AST_NODE(p->data), out);
        if (g_slist_next(p) != NULL)
        {
            fprintf(out, ",");
        }
    }
    fprintf(out, "]");
}

static void
ast_array_literal_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_array_literal_do_print;
}

