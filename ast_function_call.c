#include <string.h>

#include "ast_function_call.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_function_call_do_print(AstNode *self, FILE *out);

static void
ast_function_call_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_function_call_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstFunctionCallClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_function_call_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstFunctionCall),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_EXPRESSION,
                                    "AstFunctionCallType",
                                    &info, 0);
    }
    return type;
}

AstFunctionCall *
ast_function_call_new(char *name,
                      GSList *arguments,
                      guint line_number)
{
    AstFunctionCall *obj;

    obj = g_object_new(XDP_TYPE_AST_FUNCTION_CALL,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->name = strdup(name);
    obj->arguments = arguments;

    return obj;
}

char *
ast_function_call_get_name(AstFunctionCall *self)
{
    assert(self);
    assert(XDP_IS_AST_FUNCTION_CALL(self));

    return self->name;
}

GSList *
ast_function_call_get_arguments(AstFunctionCall *self)
{
    assert(self);
    assert(XDP_IS_AST_FUNCTION_CALL(self));

    return self->arguments;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_function_call_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_FUNCTION_CALL(self));
    assert(out);

    AstFunctionCall *func_call = (AstFunctionCall *)self;
    GSList *p = func_call->arguments;

    fprintf(out, "%s(", func_call->name);
    while(p != NULL)
    {
        ast_node_print(XDP_AST_NODE(p->data), out);
        if (p->next != NULL)
        {
            fprintf(out, ", ");
        }
        p = p->next;
    }
    fprintf(out, ")");
}

static void
ast_function_call_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_function_call_do_print;
}

