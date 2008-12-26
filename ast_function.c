#include <string.h>

#include "ast_function.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_function_do_print(AstNode *self, FILE *out);

static void
ast_function_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstFunctionClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_function_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstFunction),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_NODE,
                                    "AstFunctionType",
                                    &info, 0);
    }
    return type;
}

AstFunction* 
ast_function_new(char *name, 
                 GSList *parameters,
                 AstDataType *return_type,
                 AstCodeBlock *body)
{
    AstFunction *func;

    func = g_object_new(XDP_TYPE_AST_FUNCTION, NULL);
    func->name = strdup(name);
    func->parameters = parameters;
    func->return_type = return_type;
    func->body = body;

    return func;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_function_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);

    AstFunction *func = (AstFunction *)self;
    fprintf(out, "function [%p]\n"         \
                 "  name: %s return type:",
                 self, func->name);

    ast_node_print(XDP_AST_NODE(func->return_type), out);
    fprintf(out, "\n  params: ");

    GSList *p = func->parameters;
    while(p != NULL)
    {
        ast_node_print(XDP_AST_NODE(p->data), out);
        if (p->next != NULL)
        {
            fprintf(out, ", ");
        }
        p = p->next;
    }
    fprintf(out, "\n");
    ast_node_print(XDP_AST_NODE(func->body), out);
}

static void
ast_function_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_function_do_print;
}

