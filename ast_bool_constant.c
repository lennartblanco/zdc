#include "ast_bool_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_bool_constant_do_print(AstNode *self, FILE *out);

static void
ast_bool_constant_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_bool_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstBoolConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_bool_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstBoolConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_CONSTANT,
                                    "AstBoolConstantType",
                                    &info, 0);
    }
    return type;
}

AstBoolConstant *
ast_bool_constant_new(bool value)
{
    AstBoolConstant *obj;

    obj = g_object_new(AST_TYPE_BOOL_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

bool
ast_bool_constant_get_value(AstBoolConstant *self)
{
    assert(AST_IS_BOOL_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_bool_constant_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_BOOL_CONSTANT(self));
    assert(out);

    bool val = ((AstBoolConstant*)self)->value;

    fprintf(out, "%s", (val ? "true" : "false"));
}

static void
ast_bool_constant_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_bool_constant_do_print;
}

