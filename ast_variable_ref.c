#include <stdbool.h>

#include "ast_variable_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    XDP_AST_VARIABLE_REF_NAME = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_variable_ref_class_init(gpointer klass, gpointer foo);

static void
ast_variable_ref_do_print(AstNode *self, FILE *out);

static void
ast_variable_ref_set_property(GObject *object,
                              guint property_id,
                              const GValue *value,
                              GParamSpec *pspec);

static void
ast_variable_ref_get_property(GObject *object,
                              guint property_id,
                              GValue *value,
                              GParamSpec *pspec);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_variable_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstVariableRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_variable_ref_class_init,  /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstVariableRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_EXPRESSION,
                                    "AstVariableRefType",
                                    &info, 0);
    }
    return type;
}

AstVariableRef* 
ast_variable_ref_new(char *name)
{
    AstVariableRef *ref;

    ref = g_object_new(XDP_TYPE_AST_VARIABLE_REF,
                       "ast-variable-ref-name", name,
                       NULL);
    return ref;
}

char *
ast_variable_ref_get_name(AstVariableRef *self)
{
    assert(self);
    assert(XDP_IS_AST_VARIABLE_REF(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_variable_ref_class_init(gpointer klass, gpointer foo)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ast_variable_ref_set_property;
    gobject_class->get_property = ast_variable_ref_get_property;

    /*
     * install 'name' property 
     */
    pspec = g_param_spec_string("ast-variable-ref-name",
                                "ast variable name",
                                "the name of the variable",
                                "no-name-set" /* default value */,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    XDP_AST_VARIABLE_REF_NAME,
                                    pspec);

    /* override 'print()' method */
    ((AstNodeClass *)klass)->do_print = ast_variable_ref_do_print;

}

static void
ast_variable_ref_set_property(GObject *object,
                              guint property_id,
                              const GValue *value,
                              GParamSpec *pspec)
{
    /* we only have one property */
    assert(property_id == XDP_AST_VARIABLE_REF_NAME);
    AstVariableRef *varref = XDP_AST_VARIABLE_REF(object);

    varref->name = g_value_dup_string(value);    
}

static void
ast_variable_ref_get_property(GObject *object,
                              guint property_id,
                              GValue *value,
                              GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static void
ast_variable_ref_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_VARIABLE_REF(self));
    assert(out);

    fprintf(out, "%s",
            XDP_AST_VARIABLE_REF(self)->name);
}


