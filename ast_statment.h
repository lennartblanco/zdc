#ifndef AST_STATMENT_INC_X
#define AST_STATMENT_INC_X

#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_STATMENT ast_statment_get_type()

#define AST_STATMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_STATMENT, AstStatment))

#define AST_STATMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_STATMENT, AstStatmentClass))

#define AST_IS_STATMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_STATMENT))

#define AST_IS_STATMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_STATMENT))

#define AST_STATMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_STATMENT, AstStatmentClass))

typedef struct
{
    AstNode parent;
} AstStatment;

typedef struct
{
    AstNodeClass parent_class;
} AstStatmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_statment_get_type(void);

#endif /* AST_STATMENT_INC_X */
