#ifndef AST_ARRAY_SLICE_REF_INC_X
#define AST_ARRAY_SLICE_REF_INC_X

#include "ast_ident.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ARRAY_SLICE_REF ast_array_slice_ref_get_type()

#define AST_ARRAY_SLICE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ARRAY_SLICE_REF, AstArraySliceRef))

#define AST_ARRAY_SLICE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ARRAY_SLICE_REF, AstArraySliceRefClass))

#define AST_IS_ARRAY_SLICE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ARRAY_SLICE_REF))

#define AST_IS_ARRAY_SLICE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ARRAY_SLICE_REF))

#define AST_ARRAY_SLICE_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ARRAY_SLICE_REF, AstArraySliceRefClass))

typedef struct
{
    AstExpression parent;

    /* private */
    AstExpression *array;
    AstExpression *start;
    AstExpression *end;
} AstArraySliceRef;

typedef struct
{
    AstIdentClass parent_class;
} AstArraySliceRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_array_slice_ref_get_type(void);

AstArraySliceRef *
ast_array_slice_ref_new(AstExpression *array,
                        AstExpression *start,
                        AstExpression *end,
                        guint line_number);
AstExpression *
ast_array_slice_ref_get_array(AstArraySliceRef *self);

AstExpression *
ast_array_slice_ref_get_start(AstArraySliceRef *self);

AstExpression *
ast_array_slice_ref_get_end(AstArraySliceRef *self);

#endif /* AST_ARRAY_SLICE_REF_INC_X */
