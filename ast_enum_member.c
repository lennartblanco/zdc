#include "ast_enum_member.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_enum_member_do_print(AstNode *self, FILE *out);

static void
ast_enum_member_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_enum_member_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstEnumMemberClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_enum_member_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstEnumMember),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstEnumMemberType",
                                    &info, 0);
    }
    return type;
}

AstEnumMember * 
ast_enum_member_new(DtDataType *type, gchar *name, AstExpression *initializer)
{ 
    AstEnumMember *obj;

    obj = g_object_new(AST_TYPE_ENUM_MEMBER, NULL);

    obj->type = type;
    obj->name = g_strdup(name);
    obj->initializer = initializer;

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_enum_member_do_print(AstNode *self, FILE *out)
{
    assert(AST_IS_ENUM_MEMBER(self));
    assert(out);

    AstEnumMember *emem = AST_ENUM_MEMBER(self);

    if (emem->type != NULL)
    {
        fprintf(out, "%s ", dt_data_type_get_string(emem->type));
    }
    fprintf(out, "%s", emem->name);

    if (emem->initializer)
    {
        fprintf(out, " = ");
        ast_node_print(AST_NODE(emem->initializer), out);
    }
    fprintf(out, "\n");
}

static void
ast_enum_member_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_enum_member_do_print;
}

