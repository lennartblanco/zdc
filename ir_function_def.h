#ifndef IR_FUNCTION_DEF_INC_X
#define IR_FUNCTION_DEF_INC_X

#include "ir_function.h"
#include "ir_module.h"
#include "ir_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FUNCTION_DEF ir_function_def_get_type()

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
    IrFunctionClass parent_class;
} IrFunctionDefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_def_get_type(void);

IrFunctionDef *
ir_function_def(void *obj);

/**
 * Create new IR function object.
 *
 * @param return_type  return type of the function
 * @param name         name of the function
 * @param parent_scope the symbol table of the semantical scope where the
 *                     function is declared
 *
 * @return object representing an IR function
 */
IrFunctionDef *
ir_function_def_new(ir_linkage_type_t linkage_type,
                    bool is_private,
                    DtDataType *return_type,
                    char *name,
                    GSList *parameters,
                    IrModule *parent_module,
                    IrScope *scope,
                    guint line_number);

char *
ir_function_def_get_name(IrFunctionDef *self);

/**
 * Get the function formal parameters.
 */
GSList *
ir_function_def_get_parameters(IrFunctionDef *self);

IrVariable *
ir_function_def_get_this_param(IrFunctionDef *self);

void
ir_function_def_set_this_type(IrFunctionDef *self, DtDataType *this_type);

void
ir_function_def_set_return_type(IrFunctionDef *self, DtDataType *type);

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
