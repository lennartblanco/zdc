#include "ast_array_slice_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_array_slice_ref_do_print(AstNode *self, FILE *out);

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
      type = g_type_register_static(XDP_TYPE_AST_VARIABLE_REF,
                                    "AstArraySliceRefType",
                                    &info, 0);
    }
    return type;
}

AstArraySliceRef *
ast_array_slice_ref_new(char *name,
                        AstExpression *start,
                        AstExpression *end)
{
    AstArraySliceRef *obj;

    obj = g_object_new(XDP_TYPE_AST_ARRAY_SLICE_REF,
                       "ast-variable-ref-name", name,
                       NULL);
    obj->start = start;
    obj->end = end;

    return obj;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_array_slice_ref_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_ARRAY_SLICE_REF(self));
    assert(out);

    AstArraySliceRef *ref = XDP_AST_ARRAY_SLICE_REF(self);
    fprintf(out, "%s[",
            ast_variable_ref_get_name(XDP_AST_VARIABLE_REF(self)));
    if (ref->start != NULL && ref->end != NULL)
    {
        ast_node_print(XDP_AST_NODE(ref->start), out);
        fprintf(out, "..");
        ast_node_print(XDP_AST_NODE(ref->end), out);
    }
    fprintf(out, "]");
}

static void
ast_array_slice_ref_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_array_slice_ref_do_print;
}

