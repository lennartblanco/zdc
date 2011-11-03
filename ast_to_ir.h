#ifndef AST_TO_IR_INC_X
#define AST_TO_IR_INC_X

#include "ast_module.h"
#include "ir_module.h"
#include "sem_analyze.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Convert AST representation of a module definition to IR form.
 */
IrModule *
ast_module_to_ir(compilation_status_t *compile_status, AstModule *ast_module);

#endif /* AST_TO_IR_INC_X */
