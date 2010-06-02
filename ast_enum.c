#include "ast_enum.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_enum_do_print(AstNode *self, FILE *out, int indention);

static void
ast_enum_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_enum_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstEnumClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_enum_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstEnum),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstEnumType",
                                    &info, 0);
    }
    return type;
}

AstEnum *
ast_enum_new(gchar *tag,
             DtDataType *base_type,
             GSList *members,
             guint line_number)
{
    assert(members != NULL);

    AstEnum *obj;

    obj = g_object_new(AST_TYPE_ENUM,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->tag = g_strdup(tag);
    obj->base_type = base_type;
    obj->members = members;

    return obj;
}

gchar *
ast_enum_get_tag(AstEnum *self)
{
    assert(AST_IS_ENUM(self));

    return self->tag;
}

DtDataType *
ast_enum_get_base_type(AstEnum *self)
{
    assert(AST_IS_ENUM(self));

    return self->base_type;
}

GSList *
ast_enum_get_members(AstEnum *self)
{
    assert(AST_IS_ENUM(self));

    return self->members;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_enum_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_ENUM(self));
    assert(out);

    AstEnum *e = AST_ENUM(self);
    GSList *i;

    fprintf(out, "  enum\n");
    fprintf(out, "    tag: %s\n",
            e->tag ? e->tag : "<anonymous>");
    fprintf(out,
            "    base type: %s\n"
            "    members:\n",
            e->base_type ? dt_data_type_get_string(e->base_type) :
                           "<unspecified>");
    
    for (i = e->members; i != NULL; i = g_slist_next(i))
    {
        fprintf(out, "      ");
        ast_node_print(i->data, out, indention);
    }
}

static void
ast_enum_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_enum_do_print;
}

