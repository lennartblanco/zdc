#ifndef IR_FUNCTION_DEF_INC_X
#define IR_FUNCTION_DEF_INC_X

#include "ir_function.h"
#include "ir_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FUNCTION_DEF ir_function_def_get_type()

#define IR_FUNCTION_DEF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FUNCTION_DEF, IrFunctionDef))

#define IR_FUNCTION_DEF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FUNCTION_DEF, IrFunctionDefClass))

#define IR_IS_FUNCTION_DEF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FUNCTION_DEF))

#define IR_IS_FUNCTION_DEF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FUNCTION_DEF))

#define IR_FUNCTION_DEF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FUNCTION_DEF, IrFunctionDefClass))

typedef struct
{
    IrFunction parent;
    /* private */
    sym_table_t  *param_symbols;
    IrCodeBlock  *body;
} IrFunctionDef;

typedef struct
{
  IrFunctionClass parent_class;
} IrFunctionDefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_def_get_type(void);


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
IrFunctionDef *
ir_function_def_new(DtDataType *return_type,
                    char *name,
                    GSList *parameters,
                    sym_table_t *parent_scope);

char *
ir_function_def_get_name(IrFunctionDef *self);

/**
 * Get the function formal parameters.
 *
 * @return formal parameters as a list of AstVariableDeclaration pointer
 */
GSList *
ir_function_def_get_parameters(IrFunctionDef *self);

DtDataType *
ir_function_def_get_return_type(IrFunctionDef *self);

/**
 * Get symbol table that contains function parameters.
 */
sym_table_t *
ir_function_def_get_parameter_symbols(IrFunctionDef *self);

IrCodeBlock *
ir_function_def_get_body(IrFunctionDef *self);

#endif /* IR_FUNCTION_DEF_INC_X */
