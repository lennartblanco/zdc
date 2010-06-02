#include "ast_array_cell_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_array_cell_ref_do_print(AstNode *self, FILE *out, int indention);

static void
ast_array_cell_ref_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_array_cell_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstArrayCellRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_array_cell_ref_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstArrayCellRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_VARIABLE_REF,
                                    "AstArrayCellRefType",
                                    &info, 0);
    }
    return type;
}

AstArrayCellRef *
ast_array_cell_ref_new(char *name,
                       AstExpression *index,
                       guint line_number)
{
    AstArrayCellRef *obj;

    obj = g_object_new(AST_TYPE_ARRAY_CELL_REF,
                       "ast-node-line-number", line_number,
                       "ast-variable-ref-name", name,
                       NULL);
    obj->index = index;

    return obj;
}

char *
ast_array_cell_ref_get_name(AstArrayCellRef *self)
{
    assert(AST_IS_ARRAY_CELL_REF(self));

    return ast_variable_ref_get_name(AST_VARIABLE_REF(self));
}

AstExpression *
ast_array_cell_ref_get_index(AstArrayCellRef *self)
{
    assert(AST_IS_ARRAY_CELL_REF(self));

    return self->index;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_array_cell_ref_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_ARRAY_CELL_REF(self));
    assert(out);

    AstArrayCellRef *ref = AST_ARRAY_CELL_REF(self);
    fprintf(out, "%s[", ast_variable_ref_get_name(AST_VARIABLE_REF(self)));
    ast_node_print(AST_NODE(ref->index), out, indention);
    fprintf(out, "]");
}

static void
ast_array_cell_ref_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_array_cell_ref_do_print;
}

