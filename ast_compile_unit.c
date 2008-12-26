#include "ast_compile_unit.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_compile_unit_do_print(AstNode *self, FILE *out);

static void
ast_compile_unit_class_init(gpointer klass, gpointer unused);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_compile_unit_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstCompileUnitClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_compile_unit_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstCompileUnit),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_NODE,
                                    "AstCompileUnitType",
                                    &info, 0);
    }
    return type;
}

AstCompileUnit*
ast_compile_unit_new (void)
{
    AstCompileUnit *node;

    node = g_object_new(XDP_TYPE_AST_COMPILE_UNIT, NULL);
    node->functions = NULL;

    return node;
}

void
ast_compile_unit_add_function(AstCompileUnit *self,
                              AstFunction *function)
{
    assert(self);
    assert(function);

    self->functions = g_slist_append(self->functions, function);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_compile_unit_do_print(AstNode *self, FILE *out)
{
    assert(XDP_IS_AST_COMPILE_UNIT(self));
    AstCompileUnit *comp_unit = (AstCompileUnit *)self;
    fprintf(out, "compile unit [%p]\n", comp_unit);
    GSList *p = comp_unit->functions;

    while(p != NULL)
    {
        ast_node_print(XDP_AST_NODE(p->data), out);
        p = p->next;
    }
}

static void
ast_compile_unit_class_init(gpointer klass, gpointer unused)
{
    ((AstNodeClass *)klass)->do_print = ast_compile_unit_do_print;
}
