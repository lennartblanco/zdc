#ifndef AST_AUTO_TYPE_INC_X
#define AST_AUTO_TYPE_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_AUTO_TYPE ast_auto_type_get_type()

#define XDP_AST_AUTO_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_AUTO_TYPE, AstAutoType))

#define XDP_AST_AUTO_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_AUTO_TYPE, AstAutoTypeClass))

#define XDP_IS_AST_AUTO_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_AUTO_TYPE))

#define XDP_IS_AST_AUTO_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_AUTO_TYPE))

#define XDP_AST_AUTO_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_AUTO_TYPE, AstAutoTypeClass))

typedef struct
{
    DtDataType parent;
} AstAutoType;

typedef struct
{
    DtDataTypeClass parent_class;
} AstAutoTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_auto_type_get_type(void);

AstAutoType * 
ast_auto_type_new();

#endif /* AST_AUTO_TYPE_INC_X */
