#ifndef IML_OPERATION_INC_X
#define IML_OPERATION_INC_X

#include <stdio.h>

#include "iml_operand.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

/*
 * IML operations
 */
typedef enum iml_opcode_e
{
    iml_add,
    iml_sub,
    iml_smult,
    iml_umult,
    iml_equal,
    iml_nequal,
    iml_sless,
    iml_uless,
    iml_sgreater,
    iml_ugreater,
    iml_slesseq,
    iml_ulesseq,
    iml_sgreatereq,
    iml_ugreatereq,
    iml_copy,
    iml_call,
    iml_call_c,
    iml_return,
    iml_cast,
} iml_opcode_t;

typedef struct iml_operation_s iml_operation_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_operation_t *
iml_operation_new(iml_opcode_t operation, ...);

iml_opcode_t
iml_operation_get_opcode(iml_operation_t *self);

/**
 * Get this operations n-th operand.
 *
 * @param operand_num operands number, first operand is 1
 */
void *
iml_operation_get_operand(iml_operation_t *self,
		                  guint operand_num);

void
iml_operation_print(iml_operation_t *self,
                    FILE *out,
                    int indention);

#endif /* IML_OPERATION_INC_X */
