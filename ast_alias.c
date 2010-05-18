#include "ast_alias.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_alias_do_print(AstNode *self, FILE *out);

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
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstAliasType",
                                    &info, 0);
    }
    return type;
}

AstAlias * 
ast_alias_new(DtDataType *type, gchar *name)
{
    assert(name);
    assert(DT_IS_DATA_TYPE(type));

    AstAlias *obj;

    obj = g_object_new(AST_TYPE_ALIAS, NULL);

    obj->type = type;
    obj->name = g_strdup(name);

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_alias_do_print(AstNode *self, FILE *out)
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

