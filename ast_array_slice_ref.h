#ifndef AST_ARRAY_SLICE_REF_INC_X
#define AST_ARRAY_SLICE_REF_INC_X

#include "ast_variable_ref.h"

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
                        AstExpression *end,
                        guint line_number);
char *
ast_array_slice_ref_get_name(AstArraySliceRef *self);

AstExpression *
ast_array_slice_ref_get_start(AstArraySliceRef *self);

AstExpression *
ast_array_slice_ref_get_end(AstArraySliceRef *self);

#endif /* AST_ARRAY_SLICE_REF_INC_X */
