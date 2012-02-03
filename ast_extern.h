#ifndef AST_EXTERN_INC_X
#define AST_EXTERN_INC_X

#include "ast_attribute.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_EXTERN ast_extern_get_type()

#define AST_EXTERN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_EXTERN, AstExtern))

#define AST_EXTERN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_EXTERN, AstExternClass))

#define AST_IS_EXTERN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_EXTERN))

#define AST_IS_EXTERN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_EXTERN))

#define AST_EXTERN_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_EXTERN, AstExternClass))

typedef struct
{
    AstAttribute parent;

    /* private */
    const char *linkage;
} AstExtern;

typedef struct
{
    AstAttributeClass parent_class;
} AstExternClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_extern_get_type(void);

AstExtern *
ast_extern_new(const char *linkage, guint line_number);

const char *
ast_extern_get_linkage(AstExtern *self);

#endif /* AST_EXTERN_INC_X */
