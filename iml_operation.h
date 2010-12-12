#ifndef IML_OPERATION_INC_X
#define IML_OPERATION_INC_X

#include <stdio.h>

#include "iml_operand.h"
#include "iml_variable.h"

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
    iml_and,
    iml_or,
    iml_ineg,
    iml_bneg,
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
    iml_mset,
    iml_setfld,
    iml_getfld,
    iml_getaddr,
    iml_jmp,
    iml_jmpneq,
    iml_call,
    iml_call_c,
    iml_return,
    iml_cast,
    iml_label
} iml_opcode_t;

typedef struct iml_operation_s iml_operation_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_operation_t *
iml_operation_new(iml_opcode_t operation, ...);

/**
 * utility function for create an iml_call_c operation.
 *
 * @param function the name of the function to call
 * @param res destination for the return value of function,
 *             or NULL for calls to void function
 * @param ... the list of arguments to the function, as ImlOperand objects,
 *            terminated by NULL value
 */
iml_operation_t *
iml_operation_new_call_c(gchar *function, ImlVariable *res, ...);

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
