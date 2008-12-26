#include "ast_return.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_return_do_print(AstNode *self, FILE *out);

static void
ast_return_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_return_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstReturnClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_return_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstReturn),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_STATMENT,
                                    "AstReturnType",
                                    &info, 0);
    }
    return type;
}

AstReturn * 
ast_return_new(AstExpression *return_value)
{ 
    AstReturn *ret;

    ret = g_object_new(XDP_TYPE_AST_RETURN, NULL);
    ret->return_value = return_value;

    return ret;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_return_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);
    assert(XDP_IS_AST_RETURN(self));
    fprintf(out, "  return ");
    AstReturn *ret = (AstReturn *)self;
    if (ret->return_value != NULL)
    {
        ast_node_print(XDP_AST_NODE(ret->return_value), out);
    }
    fprintf(out, "\n");
}

static void
ast_return_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_return_do_print;
}

