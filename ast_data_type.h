#ifndef AST_DATA_TYPE_INC_X
#define AST_DATA_TYPE_INC_X

#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_DATA_TYPE ast_data_type_get_type()

#define XDP_AST_DATA_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_DATA_TYPE, AstDataType))

#define XDP_AST_DATA_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_DATA_TYPE, AstDataTypeClass))

#define XDP_IS_AST_DATA_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_DATA_TYPE))

#define XDP_IS_AST_DATA_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_DATA_TYPE))

#define XDP_AST_DATA_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_DATA_TYPE, AstDataTypeClass))

typedef struct
{
    AstNode parent;
} AstDataType;

typedef struct
{
    AstNodeClass parent_class;
} AstDataTypeClass;

typedef enum basic_data_type_e
{
    void_type, 
    bool_type,               /* boolean */
    int_type                 /* int 32-bit signed */
} basic_data_type_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_data_type_get_type(void);

#endif /* AST_DATA_TYPE_INC_X */
