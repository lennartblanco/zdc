#include "ast_struct.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_struct_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstStructClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstStruct),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_USER_TYPE,
                                    "AstStructType",
                                    &info, 0);
    }
    return type;
}

AstStruct *
ast_struct_new(gchar *name,
               GSList *members,
               GSList *methods,
               bool opaque,
               guint line_number)
{
    AstStruct *obj;

    obj = g_object_new(AST_TYPE_STRUCT,
                       "ast-node-line-number", line_number,
                       NULL);

    /* no members/methods should be present for opaque struct declarations */
    assert(!opaque || (members == NULL && methods == NULL));

    obj->name = g_strdup(name);
    obj->members = members;
    obj->methods = methods;
    obj->opaque = opaque;

    return obj;
}

gchar *
ast_struct_get_name(AstStruct *self)
{
    assert(AST_IS_STRUCT(self));

    return self->name;
}

GSList *
ast_struct_get_members(AstStruct *self)
{
    assert(AST_IS_STRUCT(self));

    return self->members;
}

GSList *
ast_struct_get_methods(AstStruct *self)
{
    assert(AST_IS_STRUCT(self));

    return self->methods;
}

bool
ast_struct_is_opaque(AstStruct *self)
{
    assert(AST_IS_STRUCT(self));

    return self->opaque;
}
