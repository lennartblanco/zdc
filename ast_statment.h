#ifndef AST_STATMENT_INC_X
#define AST_STATMENT_INC_X

#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_STATMENT ast_statment_get_type()

#define XDP_AST_STATMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_STATMENT, AstStatment))

#define XDP_AST_STATMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_STATMENT, AstStatmentClass))

#define XDP_IS_AST_STATMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_STATMENT))

#define XDP_IS_AST_STATMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_STATMENT))

#define XDP_AST_STATMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_STATMENT, AstStatmentClass))

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

GType ast_statment_get_type(void);

#endif /* AST_STATMENT_INC_X */
