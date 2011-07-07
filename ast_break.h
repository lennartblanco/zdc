#ifndef AST_BREAK_INC_X
#define AST_BREAK_INC_X

#include "ast_statment.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_BREAK ast_break_get_type()

#define AST_BREAK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_BREAK, AstBreak))

#define AST_BREAK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_BREAK, AstBreakClass))

#define AST_IS_BREAK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_BREAK))

#define AST_IS_BREAK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_BREAK))

#define AST_BREAK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_BREAK, AstBreakClass))

typedef struct
{
    AstStatment    parent;
} AstBreak;

typedef struct
{
    AstStatmentClass parent_class;
} AstBreakClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_break_get_type(void);

AstBreak *
ast_break_new(guint line_number);

#endif /* AST_BREAK_INC_X */
