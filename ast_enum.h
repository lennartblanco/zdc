#ifndef AST_ENUM_INC_X
#define AST_ENUM_INC_X

#include "ast_node.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ENUM ast_enum_get_type()

#define AST_ENUM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ENUM, AstEnum))

#define AST_ENUM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ENUM, AstEnumClass))

#define AST_IS_ENUM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ENUM))

#define AST_IS_ENUM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ENUM))

#define AST_ENUM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ENUM, AstEnumClass))

typedef struct
{
    AstNode    parent;

    /* private */
    gchar *tag;
    DtDataType *base_type;
    GSList *members;
} AstEnum;

typedef struct
{
    AstNodeClass parent_class;
} AstEnumClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_enum_get_type(void);

/**
 * @param tag enum tag, or NULL if anonymous
 * @param base_type enum base type, or NULL if none is specified
 * @param members enum members, as list of AstEnumMember objects
 */
AstEnum *
ast_enum_new(gchar *tag,
             DtDataType *base_type,
             GSList *members,
             guint line_number);

gchar *
ast_enum_get_tag(AstEnum *self);

DtDataType *
ast_enum_get_base_type(AstEnum *self);

GSList *
ast_enum_get_members(AstEnum *self);

#endif /* AST_ENUM_INC_X */
