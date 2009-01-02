#ifndef AST_ARRAY_CELL_REF_INC_X
#define AST_ARRAY_CELL_REF_INC_X

#include "ast_variable_ref.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_ARRAY_CELL_REF ast_array_cell_ref_get_type()

#define XDP_AST_ARRAY_CELL_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_ARRAY_CELL_REF, AstArrayCellRef))

#define XDP_AST_ARRAY_CELL_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_ARRAY_CELL_REF, AstArrayCellRefClass))

#define XDP_IS_AST_ARRAY_CELL_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_ARRAY_CELL_REF))

#define XDP_IS_AST_ARRAY_CELL_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_ARRAY_CELL_REF))

#define XDP_AST_ARRAY_CELL_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_ARRAY_CELL_REF, AstArrayCellRefClass))

typedef struct
{
    AstVariableRef parent;

    /* private */
    char          *name;
    int           index;
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
ast_array_cell_ref_new(char *name, int index);

char *
ast_array_cell_get_name(AstArrayCellRef *self);

int
ast_array_cell_get_index(AstArrayCellRef *self);

#endif /* AST_ARRAY_CELL_REF_INC_X */
