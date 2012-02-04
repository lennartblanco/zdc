#ifndef AST_PROTECTION_INC_X
#define AST_PROTECTION_INC_X

#include <stdbool.h>

#include "ast_attribute.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_PROTECTION ast_protection_get_type()

#define AST_PROTECTION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_PROTECTION, AstProtection))

#define AST_PROTECTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_PROTECTION, AstProtectionClass))

#define AST_IS_PROTECTION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_PROTECTION))

#define AST_IS_PROTECTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_PROTECTION))

#define AST_PROTECTION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_PROTECTION, AstProtectionClass))

typedef struct
{
    AstAttribute parent;

    /* private */
    bool         is_private;
} AstProtection;

typedef struct
{
    AstAttributeClass parent_class;
} AstProtectionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_protection_get_type(void);

AstProtection *
ast_protection_new(bool is_private, guint line_number);

bool
ast_protection_is_private(AstProtection *self);

#endif /* AST_PROTECTION_INC_X */
