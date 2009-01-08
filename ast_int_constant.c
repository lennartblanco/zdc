#include "ast_int_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_int_constant_do_print(AstNode *self, FILE *out);

static void
ast_int_constant_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_int_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstIntConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_int_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstIntConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_CONSTANT,
                                    "AstIntConstantType",
                                    &info, 0);
    }
    return type;
}

AstIntConstant *
ast_int_constant_new(gint32 value)
{
    AstIntConstant *obj;

    obj = g_object_new(XDP_TYPE_AST_INT_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

gint32
ast_int_constant_get_value(AstIntConstant *self)
{
    assert(self);
    assert(XDP_IS_AST_INT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_int_constant_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(out);
    assert(XDP_IS_AST_INT_CONSTANT(self));
    fprintf(out, "%d", XDP_AST_INT_CONSTANT(self)->value);
}

static void
ast_int_constant_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_int_constant_do_print;
}

