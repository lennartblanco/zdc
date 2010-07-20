#include <glib.h>
#include <stdbool.h>
#include "iml_operation.h"
#include "utils.h"

#include <assert.h>

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

void
iml_operation_print(iml_operation_t *self,
                    FILE *out,
                    int indention)
{
  assert(self);

  switch (self->opcode)
  {
    case iml_vreturn:
      fprintf_indent(out, indention, "vreturn\n");
      break;
    default:
      /* unexpected opcode */
      assert(false);
  }
}
