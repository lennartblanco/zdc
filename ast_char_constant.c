#include "ast_char_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_char_constant_do_print(AstNode *self, FILE *out);

static void
ast_char_constant_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_char_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstCharConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_char_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstCharConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_CONSTANT,
                                    "AstCharConstantType",
                                    &info, 0);
    }
    return type;
}

AstCharConstant *
ast_char_constant_new(guint8 value, guint line_number)
{
    AstCharConstant *obj;

    obj = g_object_new(AST_TYPE_CHAR_CONSTANT,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->value = value;

    return obj;
}

guint8
ast_char_constant_get_value(AstCharConstant *self)
{
    assert(AST_IS_CHAR_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_char_constant_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_CHAR_CONSTANT(self));
    assert(out);
    fprintf(out, "%c", AST_CHAR_CONSTANT(self)->value);
}

static void
ast_char_constant_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_char_constant_do_print;
}

