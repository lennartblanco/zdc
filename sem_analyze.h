#ifndef SEM_ANALYZE_INC_X
#define SEM_ANALYZE_INC_X

#include "auxil.h"
#include "ast_module.h"
#include "ir_module.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct compilation_status_s
{
    IrModule   *module;              /** current module */
    IrFunctionDef *function;         /** current function */
    const char *source_file;         /** current source file name */
    arch_backend_t *backend;         /** backend hooks */
    guint errors_count;
} compilation_status_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Preform semantic analysis of the provided module in AST-form
 * and return IR representation of the module analyzed.
 *
 * @param source_file      The source file name to use in error and warning 
 *                         messages.
 * @param get_registers    Function to use for fetching the lists of
 *                         available registers in the target architecture
 * @param ast_module       The AST module to analyze.
 *
 * @return                 module in IR form, or NULL if there were fatal
 *                         errors found during analysis.
 */
IrModule *
semantic_analyze(const char *source_file,
                 arch_backend_t *backend,
                 AstModule *ast_module);

#endif /* SEM_ANALYZE_INC_X */
