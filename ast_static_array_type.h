#ifndef AST_STATIC_ARRAY_TYPE_INC_X
#define AST_STATIC_ARRAY_TYPE_INC_X

#include "ast_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_STATIC_ARRAY_TYPE ast_static_array_type_get_type()

#define XDP_AST_STATIC_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_STATIC_ARRAY_TYPE, AstStaticArrayType))

#define XDP_AST_STATIC_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_STATIC_ARRAY_TYPE, AstStaticArrayTypeClass))

#define XDP_IS_AST_STATIC_ARRAY_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_STATIC_ARRAY_TYPE))

#define XDP_IS_AST_STATIC_ARRAY_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_STATIC_ARRAY_TYPE))

#define XDP_AST_STATIC_ARRAY_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_STATIC_ARRAY_TYPE, AstStaticArrayTypeClass))

typedef struct
{
    AstDataType parent;
    /* private */
    basic_data_type_t data_type;
    int length;
} AstStaticArrayType;

typedef struct
{
    AstDataTypeClass parent_class;
} AstStaticArrayTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_static_array_type_get_type(void);


AstStaticArrayType * 
ast_static_array_type_new(basic_data_type_t data_type, int length);

#endif /* AST_STATIC_ARRAY_TYPE_INC_X */
