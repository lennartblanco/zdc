#include <stdbool.h>

#include "ast_ident.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    AST_IDENT_NAME = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_ident_class_init(gpointer klass, gpointer foo);

static void
ast_ident_do_print(AstNode *self, FILE *out, int indention);

static void
ast_ident_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec);

static void
ast_ident_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_ident_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstIdentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_ident_class_init,  /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstIdent),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstIdentType",
                                    &info, 0);
    }
    return type;
}

AstIdent * 
ast_ident_new(char *name, guint line_number)
{
    AstIdent *obj;

    obj = g_object_new(AST_TYPE_IDENT,
                       "ast-node-line-number", line_number,
                       "ast-ident-name", name,
                       NULL);
    return obj;
}

char *
ast_ident_get_name(AstIdent *self)
{
    assert(AST_IS_IDENT(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_ident_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ast_ident_set_property;
    gobject_class->get_property = ast_ident_get_property;

    /*
     * install 'name' property 
     */
    pspec = g_param_spec_string("ast-ident-name",
                                "ast identifier name",
                                "the name of the identifier",
                                "no-name-set" /* default value */,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    AST_IDENT_NAME,
                                    pspec);

    /* override 'print()' method */
    ((AstNodeClass *)klass)->do_print = ast_ident_do_print;

}

static void
ast_ident_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec)
{
    /* we only have one property */
    assert(property_id == AST_IDENT_NAME);
    AstIdent *ident = AST_IDENT(object);

    ident->name = g_value_dup_string(value);    
}

static void
ast_ident_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static void
ast_ident_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_IDENT(self));
    assert(out);

    fprintf_indent(out, indention, "identifier\n  name: %s",
                   AST_IDENT(self)->name);
}

