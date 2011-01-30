#include "ast_alias.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_alias_do_print(AstNode *self, FILE *out, int indention);

static void
ast_alias_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_alias_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstAliasClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_alias_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAlias),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_USER_TYPE,
                                    "AstAliasType",
                                    &info, 0);
    }
    return type;
}

AstAlias *
ast_alias_new(DtDataType *type, gchar *name, guint line_number)
{
    assert(name);
    assert(DT_IS_DATA_TYPE(type));

    AstAlias *obj;

    obj = g_object_new(AST_TYPE_ALIAS,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->type = type;
    obj->name = g_strdup(name);

    return obj;
}

DtDataType *
ast_alias_get_data_type(AstAlias *self)
{
    assert(AST_IS_ALIAS(self));

    return self->type;
}

gchar *
ast_alias_get_name(AstAlias *self)
{
    assert(AST_IS_ALIAS(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_alias_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_ALIAS(self));
    assert(out);

    AstAlias *alias = AST_ALIAS(self);

    fprintf(out, "  alias %s %s\n",
            dt_data_type_get_string(alias->type), alias->name);
}

static void
ast_alias_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_alias_do_print;
}

