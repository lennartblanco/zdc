#ifndef SEM_ANALYZE_INC_X
#define SEM_ANALYZE_INC_X

#include "sym_table.h"
#include "ast_module.h"
#include "ir_module.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Preform semantic analysis of the provided module in AST-form
 * and return IR representation of the module analyzed.
 *
 * @param source_file      The source file name to use in error and warning 
 *                         messages.
 * @param ast_module       The AST module to analyze.
 *
 * @return                 module in IR form, or NULL if there were fatal
 *                         errors found during analysis.
 */
IrModule *
semantic_analyze(const char *source_file, AstModule *ast_module);

#endif /* SEM_ANALYZE_INC_X */
