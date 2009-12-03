#include "ast_uint_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_uint_constant_do_print(AstNode *self, FILE *out);

static void
ast_uint_constant_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_uint_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstUintConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_uint_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstUintConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_CONSTANT,
                                    "AstUintConstantType",
                                    &info, 0);
    }
    return type;
}

AstUintConstant *
ast_uint_constant_new(guint32 value, guint line_number)
{
    AstUintConstant *obj;

    obj = g_object_new(AST_TYPE_UINT_CONSTANT,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->value = value;

    return obj;
}

guint32
ast_uint_constant_get_value(AstUintConstant *self)
{
    assert(AST_IS_UINT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_uint_constant_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_UINT_CONSTANT(self));
    assert(out);
    fprintf(out, "%u", AST_UINT_CONSTANT(self)->value);
}

static void
ast_uint_constant_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_uint_constant_do_print;
}

