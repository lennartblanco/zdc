#include <stdbool.h>
#include "ast_basic_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_basic_type_do_print(AstNode *self, FILE *out);

static void
ast_basic_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_basic_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstBasicTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_basic_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstBasicType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_DATA_TYPE,
                                    "AstBasicTypeType",
                                    &info, 0);
    }
    return type;
}

AstBasicType * 
ast_basic_type_new(basic_data_type_t data_type)
{
    AstBasicType *basic_type;

    basic_type = g_object_new(XDP_TYPE_AST_BASIC_TYPE, NULL);
    basic_type->data_type = data_type;

    return basic_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_basic_type_do_print(AstNode *self, FILE *out)
{
    char *str;
    switch(((AstBasicType*)self)->data_type)
    {
        case int_type:
            str = "int";
            break;
        case void_type:
            str = "void";
            break;
        case bool_type:
            str = "bool";
            break;
        default:
            /* unexpected basic data type */
            assert(false);
    }
    fprintf(out, "%s", str);
}

static void
ast_basic_type_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_basic_type_do_print;
}

