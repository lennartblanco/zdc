#ifndef SEM_ANALYZE_VALIDATE_INC_X
#define SEM_ANALYZE_VALIDATE_INC_X

#include "auxil.h"
#include "ir_module.h"
#include "sem_analyze.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Resolve all symbols, add implicit ir-statments and check
 * that the code is semantically correct.
 */
void
sem_analyze_validate(compilation_status_t *compile_status,
                     IrModule *module);

#endif /* SEM_ANALYZE_VALIDATE_INC_X */
