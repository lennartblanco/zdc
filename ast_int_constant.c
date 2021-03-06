#include "ast_int_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_int_constant_do_print(AstNode *self, FILE *out, int indention);

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
      type = g_type_register_static(AST_TYPE_CONSTANT,
                                    "AstIntConstantType",
                                    &info, 0);
    }
    return type;
}

AstIntConstant *
ast_int_constant_new(gint32 value, guint line_number)
{
    AstIntConstant *obj;

    obj = g_object_new(AST_TYPE_INT_CONSTANT,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->value = value;

    return obj;
}

gint32
ast_int_constant_get_value(AstIntConstant *self)
{
    assert(AST_IS_INT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_int_constant_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_INT_CONSTANT(self));
    assert(out);
    fprintf(out, "%d", AST_INT_CONSTANT(self)->value);
}

static void
ast_int_constant_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_int_constant_do_print;
}

