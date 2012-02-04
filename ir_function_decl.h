#ifndef IR_FUNCTION_DECL_INC_X
#define IR_FUNCTION_DECL_INC_X

#include "ir_function.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FUNCTION_DECL ir_function_decl_get_type()

#define IR_FUNCTION_DECL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FUNCTION_DECL, IrFunctionDecl))

#define IR_FUNCTION_DECL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FUNCTION_DECL, IrFunctionDeclClass))

#define IR_IS_FUNCTION_DECL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FUNCTION_DECL))

#define IR_IS_FUNCTION_DECL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FUNCTION_DECL))

#define IR_FUNCTION_DEF_DECL_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FUNCTION_DECL, IrFunctionDeclClass))

typedef struct
{
    IrFunction parent;
    /* private */
} IrFunctionDecl;

typedef struct
{
    IrFunctionClass parent_class;
} IrFunctionDeclClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_decl_get_type(void);

IrFunctionDecl *
ir_function_decl_new(ir_linkage_type_t linkage_type,
                     bool is_private,
                     DtDataType *return_type,
                     char *name,
                     GSList *parameters,
                     IrModule *parent_module,
                     guint line_number);

#endif /* IR_FUNCTION_DECL_INC_X */
