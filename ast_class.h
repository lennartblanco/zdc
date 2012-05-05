#ifndef AST_CLASS_INC_X
#define AST_CLASS_INC_X

#include "ast_user_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_CLASS ast_class_get_type()

#define AST_CLASS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_CLASS, AstClass))

#define AST_CLASS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_CLASS, AstClassClass))

#define AST_IS_CLASS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_CLASS))

#define AST_IS_CLASS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_CLASS))

#define AST_CLASS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_CLASS, AstClassClass))

typedef struct
{
    AstUserType    parent;

    /* private */
    gchar *name;
} AstClass;

typedef struct
{
    AstUserTypeClass parent_class;
} AstClassClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_class_get_type(void);

AstClass *
ast_class_new(gchar *name,
              GSList *members,
              GSList *methods,
              guint line_number);

gchar *
ast_class_get_name(AstClass *self);

#endif /* AST_CLASS_INC_X */
