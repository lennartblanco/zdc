#include <string.h>

#include "ast_scalar_variable_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_scalar_variable_ref_do_print(AstNode *self, FILE *out);

static void
ast_scalar_variable_ref_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_scalar_variable_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstScalarVariableRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_scalar_variable_ref_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstScalarVariableRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_EXPRESSION,
                                    "AstScalarVariableRefType",
                                    &info, 0);
    }
    return type;
}

AstScalarVariableRef* 
ast_scalar_variable_ref_new(char *name)
{
    AstScalarVariableRef *ref;

    ref = g_object_new(XDP_TYPE_AST_SCALAR_VARIABLE_REF, NULL);
    ref->name = strdup(name);

    return ref;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_scalar_variable_ref_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_SCALAR_VARIABLE_REF(self));
    assert(out);

    AstScalarVariableRef *ref = (AstScalarVariableRef *)self;
    fprintf(out, "%s", ref->name);
}

static void
ast_scalar_variable_ref_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_scalar_variable_ref_do_print;
}

