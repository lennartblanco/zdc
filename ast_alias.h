#ifndef AST_ALIAS_INC_X
#define AST_ALIAS_INC_X

#include "ast_node.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ALIAS ast_alias_get_type()

#define AST_ALIAS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ALIAS, AstAlias))

#define AST_ALIAS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ALIAS, AstAliasClass))

#define AST_IS_ALIAS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ALIAS))

#define AST_IS_ALIAS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ALIAS))

#define AST_ALIAS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ALIAS, AstAliasClass))

typedef struct
{
    AstNode    parent;

    /* private */
    DtDataType *type;
    gchar *name;
} AstAlias;

typedef struct
{
  AstNodeClass parent_class;
} AstAliasClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_alias_get_type(void);

AstAlias *
ast_alias_new(DtDataType *type, gchar *name, guint line_number);

DtDataType *
ast_alias_get_data_type(AstAlias *self);

gchar *
ast_alias_get_name(AstAlias *self);

#endif /* AST_ALIAS_INC_X */
