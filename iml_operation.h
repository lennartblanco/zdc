#ifndef IML_OPERATION_INC_X
#define IML_OPERATION_INC_X

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

/*
 * IML operations
 */
typedef enum iml_opcode_e
{
    iml_return,
    iml_vreturn
} iml_opcode_t;


typedef struct iml_operation_s iml_operation_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_operation_t *
iml_operation_new(iml_opcode_t operation, ...);

#endif /* IML_OPERATION_INC_X */
