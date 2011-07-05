#include "ast_array_slice_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_array_slice_ref_do_print(AstNode *self, FILE *out, int indention);

static void
ast_array_slice_ref_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_array_slice_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstArraySliceRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_array_slice_ref_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstArraySliceRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_IDENT,
                                    "AstArraySliceRefType",
                                    &info, 0);
    }
    return type;
}

AstArraySliceRef *
ast_array_slice_ref_new(AstExpression *array,
                        AstExpression *start,
                        AstExpression *end,
                        guint line_number)
{
    AstArraySliceRef *obj;

    obj = g_object_new(AST_TYPE_ARRAY_SLICE_REF,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->array = array;
    obj->start = start;
    obj->end = end;

    return obj;
}

AstExpression *
ast_array_slice_ref_get_array(AstArraySliceRef *self)
{
   assert(AST_IS_ARRAY_SLICE_REF(self));

   return self->array;
}

AstExpression *
ast_array_slice_ref_get_start(AstArraySliceRef *self)
{
    assert(AST_IS_ARRAY_SLICE_REF(self));

    return self->start;
}

AstExpression *
ast_array_slice_ref_get_end(AstArraySliceRef *self)
{
    assert(AST_IS_ARRAY_SLICE_REF(self));


    return self->end;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_array_slice_ref_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_ARRAY_SLICE_REF(self));
    assert(out);

    AstArraySliceRef *ref = AST_ARRAY_SLICE_REF(self);
    ast_node_print(AST_NODE(ref->array), out, indention);
    fprintf(out, "[");
    if (ref->start != NULL && ref->end != NULL)
    {
        ast_node_print(AST_NODE(ref->start), out, indention);
        fprintf(out, "..");
        ast_node_print(AST_NODE(ref->end), out, indention);
    }
    fprintf(out, "]");
}

static void
ast_array_slice_ref_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_array_slice_ref_do_print;
}

