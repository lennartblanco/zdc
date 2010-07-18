#include <glib.h>
#include "iml_operation.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_operation_s
{
  iml_opcode_t opcode;
};


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_operation_t *
iml_operation_new(iml_opcode_t operation, ...)
{
    iml_operation_t *op = g_malloc(sizeof(*op));

    op->opcode = operation;

    return op;
}
