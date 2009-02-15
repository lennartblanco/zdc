#ifndef IR_FUNCTION_INC_X
#define IR_FUNCTION_INC_X

#include "ir_symbol.h"
#include "ir_code_block.h"
#include "ast_data_type.h"
#include "utils.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FUNCTION ir_function_get_type()

#define IR_FUNCTION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FUNCTION, IrFunction))

#define IR_FUNCTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FUNCTION, IrFunctionClass))

#define IR_IS_FUNCTION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FUNCTION))

#define IR_IS_FUNCTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FUNCTION))

#define IR_FUNCTION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FUNCTION, IrFunctionClass))

typedef struct
{
    IrSymbol parent;
    /* private */
    GSList       *parameters;
    sym_table_t  *param_symbols;
    AstDataType  *return_type;
    IrCodeBlock  *body;
} IrFunction;

typedef struct
{
  IrSymbolClass parent_class;
} IrFunctionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_get_type(void);


/**
 * Create new IR function object.
 *
 * @param return_type  return type of the function
 * @param name         name of the function
 * @param parameter    parameters of the function,
 *                     as a list of AstVariableDeclaration objects
 * @param parent_scope the symbol table of the semantical scope where the
 *                     function is declared
 *
 * @return object representing an IR function
 */
IrFunction *
ir_function_new(AstDataType *return_type,
                char *name,
                GSList *parameters,
                sym_table_t *parent_scope);

GSList *
ir_function_get_parameters(IrFunction *self);

/**
 * Get symbol table that contains function parameters.
 */
sym_table_t *
ir_function_get_parameter_symbols(IrFunction *self);

AstDataType *
ir_function_get_return_type(IrFunction *self);

char *
ir_function_get_name(IrFunction *self);

IrCodeBlock *
ir_function_get_body(IrFunction *self);

void
ir_function_print(IrFunction *self, FILE *out, int indention);

#endif /* IR_FUNCTION_INC_X */
