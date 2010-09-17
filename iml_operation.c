#include <glib.h>
#include <stdbool.h>
#include "iml_operation.h"
#include "iml_operand.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_operation_s
{
  iml_opcode_t opcode;
  ImlOperand *arg1;
  ImlOperand *arg2;
  ImlOperand *arg3;
};


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_operation_t *
iml_operation_new(iml_opcode_t operation, ...)
{
    va_list argp;

    iml_operation_t *op = g_malloc(sizeof(*op));

    op->opcode = operation;

    va_start(argp, operation);
    switch (operation)
    {
        case iml_vreturn:
            /* nop */
            break;
        case iml_return:
            op->arg1 = va_arg(argp, ImlOperand *);
            break;
        case iml_copy:
            op->arg1 = va_arg(argp, ImlOperand *);
            op->arg2 = va_arg(argp, ImlOperand *);
            break;
        case iml_add:
        case iml_sub:
            op->arg1 = va_arg(argp, ImlOperand *);
            op->arg2 = va_arg(argp, ImlOperand *);
            op->arg3 = va_arg(argp, ImlOperand *);
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }
    va_end(argp);

    return op;
}

iml_opcode_t
iml_operation_get_opcode(iml_operation_t *self)
{
	assert(self);

	return self->opcode;
}

ImlOperand *
iml_operation_get_operand(iml_operation_t *self,
		                  guint operand_num)
{
    assert(self);

    switch (operand_num)
    {
        case 1:
            return self->arg1;
        case 2:
            return self->arg2;
        case 3:
            return self->arg3;
    }
    /* unexpected operand number */
    assert(false);
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
        case iml_return:
            fprintf_indent(out, indention, "return ");
            iml_operand_print(self->arg1, out, 0);
            fprintf(out, "\n");
            break;
        case iml_copy:
            fprintf_indent(out, indention, "copy ");
            iml_operand_print(self->arg1, out, 0);
            fprintf(out, " => ");
            iml_operand_print(self->arg2, out, 0);
            fprintf(out, "\n");
            break;
        case iml_add:
            fprintf_indent(out, indention, "add ");
            iml_operand_print(self->arg1, out, 0);
            fprintf(out, ", ");
            iml_operand_print(self->arg2, out, 0);
            fprintf(out, " => ");
            iml_operand_print(self->arg3, out, 0);
            fprintf(out, "\n");
            break;
        case iml_sub:
            fprintf_indent(out, indention, "sub ");
            iml_operand_print(self->arg1, out, 0);
            fprintf(out, ", ");
            iml_operand_print(self->arg2, out, 0);
            fprintf(out, " => ");
            iml_operand_print(self->arg3, out, 0);
            fprintf(out, "\n");
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }
}
