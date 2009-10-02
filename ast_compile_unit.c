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
ast_compile_unit_new(void)
{
    AstCompileUnit *node;

    node = g_object_new(XDP_TYPE_AST_COMPILE_UNIT, NULL);
    node->function_defs = NULL;
    node->function_decls = NULL;

    return node;
}

void
ast_compile_unit_add_function_decl(AstCompileUnit *self,
                                   AstFunctionDecl *function_decl)
{
    assert(XDP_IS_AST_COMPILE_UNIT(self));
    assert(XDP_IS_AST_FUNCTION_DECL(function_decl));

    self->function_decls = g_slist_append(self->function_decls, function_decl);
}

void
ast_compile_unit_add_function_def(AstCompileUnit *self,
                                  AstFunctionDef *function_def)
{
    assert(XDP_IS_AST_COMPILE_UNIT(self));
    assert(XDP_IS_AST_FUNCTION_DEF(function_def));

    self->function_defs = g_slist_append(self->function_defs, function_def);
}

GSList *
ast_compile_unit_get_function_decls(AstCompileUnit *self)
{
    assert(XDP_IS_AST_COMPILE_UNIT(self));

    return self->function_decls;
}

GSList *
ast_compile_unit_get_function_defs(AstCompileUnit *self)
{
    assert(XDP_IS_AST_COMPILE_UNIT(self));

    return self->function_defs;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_compile_unit_do_print(AstNode *self, FILE *out)
{
    assert(XDP_IS_AST_COMPILE_UNIT(self));

    AstCompileUnit *comp_unit = (AstCompileUnit *)self;
    GSList *i;

    fprintf(out, "compile unit [%p]\n", comp_unit);

    for (i = comp_unit->function_defs; i != NULL; i = g_slist_next(i))
    {
        ast_node_print(XDP_AST_NODE(i->data), out);
    }

    for (i = comp_unit->function_decls; i != NULL; i = g_slist_next(i))
    {
        ast_node_print(XDP_AST_NODE(i->data), out);
    }
}

static void
ast_compile_unit_class_init(gpointer klass, gpointer unused)
{
    ((AstNodeClass *)klass)->do_print = ast_compile_unit_do_print;
}
