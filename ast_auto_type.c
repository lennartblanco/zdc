#include "ast_auto_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_auto_type_do_print(AstNode *self, FILE *out);

static void
ast_auto_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_auto_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstAutoTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_auto_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAutoType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "AstAutoTypeType",
                                    &info, 0);
    }
    return type;
}

AstAutoType *
ast_auto_type_new()
{
    AstAutoType *obj;

    obj = g_object_new(XDP_TYPE_AST_AUTO_TYPE, NULL);

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_auto_type_do_print(AstNode *self, FILE *out)
{
    fprintf(out, "auto");
}

static void
ast_auto_type_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_auto_type_do_print;
}

