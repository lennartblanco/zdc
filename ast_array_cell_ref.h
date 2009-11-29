#ifndef AST_ARRAY_CELL_REF_INC_X
#define AST_ARRAY_CELL_REF_INC_X

#include "ast_variable_ref.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ARRAY_CELL_REF ast_array_cell_ref_get_type()

#define AST_ARRAY_CELL_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ARRAY_CELL_REF, AstArrayCellRef))

#define AST_ARRAY_CELL_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ARRAY_CELL_REF, AstArrayCellRefClass))

#define AST_IS_ARRAY_CELL_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ARRAY_CELL_REF))

#define AST_IS_ARRAY_CELL_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ARRAY_CELL_REF))

#define AST_ARRAY_CELL_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ARRAY_CELL_REF, AstArrayCellRefClass))

typedef struct
{
    AstVariableRef parent;

    /* private */
    AstExpression *index;
} AstArrayCellRef;

typedef struct
{
  AstVariableRefClass parent_class;
} AstArrayCellRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_array_cell_ref_get_type(void);

AstArrayCellRef *
ast_array_cell_ref_new(char *name,
                       AstExpression *index,
                       guint line_number);

char *
ast_array_cell_ref_get_name(AstArrayCellRef *self);

AstExpression *
ast_array_cell_ref_get_index(AstArrayCellRef *self);

#endif /* AST_ARRAY_CELL_REF_INC_X */
