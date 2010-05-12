#ifndef AST_ENUM_MEMBER_INC_X
#define AST_ENUM_MEMBER_INC_X

#include "dt_data_type.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ENUM_MEMBER ast_enum_member_get_type()

#define AST_ENUM_MEMBER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ENUM_MEMBER, AstEnumMember))

#define AST_ENUM_MEMBER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ENUM_MEMBER, AstEnumMemberClass))

#define AST_IS_ENUM_MEMBER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ENUM_MEMBER))

#define AST_IS_ENUM_MEMBER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ENUM_MEMBER))

#define AST_ENUM_MEMBER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ENUM_MEMBER, AstEnumMemberClass))

typedef struct
{
    AstNode parent;

    /* private */
    DtDataType    *type;
    gchar         *name;
    AstExpression *initializer;
} AstEnumMember;

typedef struct
{
  AstNodeClass parent_class;
} AstEnumMemberClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_enum_member_get_type(void);

AstEnumMember * 
ast_enum_member_new(DtDataType *type, gchar *name, AstExpression *initializer);

#endif /* AST_ENUM_MEMBER_INC_X */
