#ifndef AST_BASIC_TYPE_INC_X
#define AST_BASIC_TYPE_INC_X

#include "ast_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_BASIC_TYPE ast_basic_type_get_type()

#define XDP_AST_BASIC_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_BASIC_TYPE, AstBasicType))

#define XDP_AST_BASIC_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_BASIC_TYPE, AstBasicTypeClass))

#define XDP_IS_AST_BASIC_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_BASIC_TYPE))

#define XDP_IS_AST_BASIC_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_BASIC_TYPE))

#define XDP_AST_BASIC_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_BASIC_TYPE, AstBasicTypeClass))

typedef struct
{
    AstDataType parent;
    /* private */
    basic_data_type_t data_type;
} AstBasicType;

typedef struct
{
    AstDataTypeClass parent_class;
} AstBasicTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_basic_type_get_type(void);


AstBasicType * 
ast_basic_type_new(basic_data_type_t data_type);

basic_data_type_t
ast_basic_type_get_data_type(AstBasicType *self);

#endif /* AST_BASIC_TYPE_INC_X */
