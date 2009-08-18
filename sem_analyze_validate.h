#ifndef SEM_ANALYZE_VALIDATE_INC_X
#define SEM_ANALYZE_VALIDATE_INC_X

#include "ir_compile_unit.h"
#include "utils.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Resolve all symbols, add implicit ir-statments and check
 * that the code is semantically correct.
 */
void
sem_analyze_validate(compilation_status_t *compile_status,
                     IrCompileUnit *compile_unit);

#endif /* SEM_ANALYZE_VALIDATE_INC_X */
