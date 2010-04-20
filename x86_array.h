#ifndef X86_ARRAY_INC_X
#define X86_ARRAY_INC_X

#include "x86.h"
#include "ir_array.h"
#include "ir_array_cell.h"
#include "ir_array_slice.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_array_handle_assigment(x86_comp_params_t *params,
                               IrAssigment *assigment,
                               sym_table_t *sym_table);
void
x86_gen_array_cell_assigment(x86_comp_params_t *params,
                             IrAssigment *assigment,
                             sym_table_t *sym_table);

void
x86_compile_array_slice_assigment(x86_comp_params_t *params,
                                  IrAssigment *assigment,
                                  sym_table_t *sym_table);

void
x86_compile_array(x86_comp_params_t *params,
                  IrArray *array,
                  sym_table_t *sym_table);

void
x86_compile_array_literal(x86_comp_params_t *params,
                          IrArrayLiteral *array_lit,
                          sym_table_t *sym_table);

void
x86_compile_array_cell(x86_comp_params_t *params,
                       IrArrayCell *array_cell,
                       sym_table_t *sym_table);

/**
 * Generate code to push an array slice onto the stack.
 */
void
x86_compile_array_slice(x86_comp_params_t *params,
                        IrArraySlice *array_slice,
                        sym_table_t *sym_table);


#endif /* X86_ARRAY_INC_X */
