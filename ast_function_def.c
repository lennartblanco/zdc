#include <string.h>

#include "ast_function_def.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_function_def_do_print(AstNode *self, FILE *out);

static void
ast_function_def_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_def_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstFunctionDefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_function_def_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstFunctionDef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_NODE,
                                    "AstFunctionDefType",
                                    &info, 0);
    }
    return type;
}

AstFunctionDef *
ast_function_def_new(char *name,
                     GSList *parameters,
                     DtDataType *return_type,
                     AstCodeBlock *body,
                     guint line_number)
{
    AstFunctionDef *func;

    func = g_object_new(XDP_TYPE_AST_FUNCTION_DEF,
                       "ast-node-line-number", line_number,
                        NULL);
    func->name = strdup(name);
    func->parameters = parameters;
    func->return_type = return_type;
    func->body = body;

    return func;
}

char *
ast_function_def_get_name(AstFunctionDef *self)
{
    assert(XDP_IS_AST_FUNCTION_DEF(self));

    return self->name;
}

GSList *
ast_function_def_get_parameters(AstFunctionDef *self)
{
    assert(XDP_IS_AST_FUNCTION_DEF(self));

    return self->parameters;
}

DtDataType *
ast_function_def_get_return_type(AstFunctionDef *self)
{
    assert(XDP_IS_AST_FUNCTION_DEF(self));

    return self->return_type;
}

AstCodeBlock *
ast_function_def_get_body(AstFunctionDef *self)
{
    assert(XDP_IS_AST_FUNCTION_DEF(self));

    return self->body;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_function_def_do_print(AstNode *self, FILE *out)
{
    assert(XDP_IS_AST_FUNCTION_DEF(self));
    assert(out);

    AstFunctionDef *func = (AstFunctionDef *)self;
    fprintf(out, "function definition [%p]\n" \
                 "  name: %s return type:",
                 self, func->name);

    dt_data_type_print(func->return_type, out);
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
ast_function_def_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_function_def_do_print;
}

