#ifndef AST_STRUCT_INC_X
#define AST_STRUCT_INC_X

#include "ast_user_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_STRUCT ast_struct_get_type()

#define AST_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_STRUCT, AstStruct))

#define AST_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_STRUCT, AstEnumClass))

#define AST_IS_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_STRUCT))

#define AST_IS_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_STRUCT))

#define AST_STRUCT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_STRUCT, AstStructClass))

typedef struct
{
    AstUserType    parent;

    /* private */
    gchar *name;
    GSList *members;
} AstStruct;

typedef struct
{
    AstNodeClass parent_class;
} AstStructClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_struct_get_type(void);

AstStruct *
ast_struct_new(gchar *name, GSList *members, guint line_number);

gchar *
ast_struct_get_name(AstStruct *self);

GSList *
ast_struct_get_members(AstStruct *self);

#endif /* AST_STRUCT_INC_X */
