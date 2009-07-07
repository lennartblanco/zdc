#ifndef SEM_ANALYZE_INC_X
#define SEM_ANALYZE_INC_X

#include "sym_table.h"
#include "ast_compile_unit.h"
#include "ir_compile_unit.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Preform semantic analysis of the provided compile unit in AST-form
 * and return IR representation of the compile unit analyzed.
 *
 * @param source_file      The source file name to use in error and warning 
 *                         messages.
 * @param ast_compile_unit The AST compile unit to analyze.
 *
 * @return                 compile unit in IR form, or NULL if there were fatal
 *                         errors found during analysis.
 */
IrCompileUnit *
semantic_analyze(const char *source_file, AstCompileUnit *ast_compile_unit);

#endif /* SEM_ANALYZE_INC_X */
