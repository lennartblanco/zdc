#ifndef X86_CODE_BLOCK_INC_X
#define X86_CODE_BLOCK_INC_X

#include "x86.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_code_block(x86_comp_params_t *params,
                       IrCodeBlock *code_block);

int
x86_code_block_assign_addrs(x86_comp_params_t *params,
                            int first_num,
                            IrCodeBlock *code_block);

#endif /* X86_CODE_BLOCK_INC_X */
