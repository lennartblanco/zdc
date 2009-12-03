#include "x86.h"
#include "ir_foreach.h"

#include <assert.h>


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_foreach(x86_comp_params_t *params,
                    IrForeach *foreach,
                    sym_table_t *sym_table)
{
    fprintf(params->out,
            "# foreach loop\n");
}



