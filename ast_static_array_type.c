#include <stdbool.h>
#include "ast_static_array_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_static_array_type_do_print(AstNode *self, FILE *out);

static void
ast_static_array_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_static_array_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstStaticArrayTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_static_array_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstStaticArrayType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_DATA_TYPE,
                                    "AstStaticArrayTypeType",
                                    &info, 0);
    }
    return type;
}

AstStaticArrayType * 
ast_static_array_type_new(basic_data_type_t data_type, int length)
{
    AstStaticArrayType *obj;

    obj = g_object_new(XDP_TYPE_AST_STATIC_ARRAY_TYPE, NULL);
    obj->data_type = data_type;
    obj->length = length;

    return obj;
}

int
ast_static_array_type_get_length(AstStaticArrayType *self)
{
    assert(self);
    assert(XDP_IS_AST_STATIC_ARRAY_TYPE(self));

    return self->length;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_static_array_type_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_STATIC_ARRAY_TYPE(self));
    assert(out);

    char *str;
    AstStaticArrayType *obj = XDP_AST_STATIC_ARRAY_TYPE(self);

    switch(obj->data_type)
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
    fprintf(out, "%s[%d]", str, obj->length);
}

static void
ast_static_array_type_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_static_array_type_do_print;
}

