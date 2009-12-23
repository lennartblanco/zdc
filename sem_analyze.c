#include "sem_analyze.h"
#include "sem_analyze_validate.h"
#include "ast_to_ir.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrModule *
semantic_analyze(const char *source_file, AstModule *ast_module)
{
    IrModule *module;

    compilation_status_t comp_stat;

    /* set-up compilation status struct */
    comp_stat.source_file = source_file;
    comp_stat.errors_count = 0;

    module = sem_analyze_ast_module_to_ir(&comp_stat, ast_module);

    /* if there were errors while converting to IR, return failure result */
    if (comp_stat.errors_count > 0)
    {
        /* @todo: clean-up comp_unit ? */
        return NULL;
    }

    sem_analyze_validate(&comp_stat, module);

    /* if there were errors during analysis, return failure result */
    if (comp_stat.errors_count > 0)
    {
        /* @todo: clean-up comp_unit ? */
        return NULL;
    }
    return module;
}


