#ifndef AST_ARRAY_SLICE_REF_INC_X
#define AST_ARRAY_SLICE_REF_INC_X

#include "ast_variable_ref.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_ARRAY_SLICE_REF ast_array_slice_ref_get_type()

#define XDP_AST_ARRAY_SLICE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_ARRAY_SLICE_REF, AstArraySliceRef))

#define XDP_AST_ARRAY_SLICE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_ARRAY_SLICE_REF, AstArraySliceRefClass))

#define XDP_IS_AST_ARRAY_SLICE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_ARRAY_SLICE_REF))

#define XDP_IS_AST_ARRAY_SLICE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_ARRAY_SLICE_REF))

#define XDP_AST_ARRAY_SLICE_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_ARRAY_SLICE_REF, AstArraySliceRefClass))

typedef struct
{
    AstVariableRef parent;

    /* private */
    AstExpression *start;
    AstExpression *end;
} AstArraySliceRef;

typedef struct
{
  AstVariableRefClass parent_class;
} AstArraySliceRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_array_slice_ref_get_type(void);

AstArraySliceRef *
ast_array_slice_ref_new(char *name,
                        AstExpression *start,
                        AstExpression *end);
AstExpression *
ast_array_slice_ref_get_start(AstArraySliceRef *self);

AstExpression *
ast_array_slice_ref_get_end(AstArraySliceRef *self);

#endif /* AST_ARRAY_SLICE_REF_INC_X */
