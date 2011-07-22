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
  void *arg1;
  void *arg2;
  void *arg3;
  void *arg4;
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
print_binary_op(iml_operation_t *op, FILE *out, int indention);

static void
print_ternary_op(iml_operation_t *op, FILE *out, int indention);

static void
print_mset_op(iml_operation_t *op, FILE *out, int indention);

static void
print_set_op(iml_operation_t *op, FILE *out, int indention);

static void
print_get_op(iml_operation_t *op, FILE *out, int indention);

static void
print_setelm_op(iml_operation_t *op, FILE *out, int indention);

static void
print_getelm_op(iml_operation_t *op, FILE *out, int indention);

static void
print_jmpcond_op(iml_operation_t *op, FILE *out, int indention);

static void
print_call_op(iml_operation_t *op, FILE *out, int indention);

static void
print_return_op(iml_operation_t *op, FILE *out, int indention);

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
        case iml_label:
            op->arg1 = va_arg(argp, char *);
            break;
        case iml_return:
        case iml_jmp:
            op->arg1 = va_arg(argp, void *);
            break;
        case iml_mset:
            op->arg1 = va_arg(argp, void *);
            op->arg2 = GUINT_TO_POINTER(va_arg(argp, guint));
            op->arg3 = va_arg(argp, void *);
            break;
        case iml_copy:
        case iml_cast:
        case iml_ineg:
        case iml_bneg:
        case iml_getaddr:
            op->arg1 = va_arg(argp, void *);
            assert(op->arg1);
            op->arg2 = va_arg(argp, void *);
            assert(op->arg2);
            break;
        case iml_add:
        case iml_sub:
        case iml_smult:
        case iml_umult:
        case iml_sdiv:
        case iml_udiv:
        case iml_smod:
        case iml_umod:
        case iml_and:
        case iml_or:
        case iml_equal:
        case iml_nequal:
        case iml_sless:
        case iml_uless:
        case iml_sgreater:
        case iml_ugreater:
        case iml_slesseq:
        case iml_ulesseq:
        case iml_sgreatereq:
        case iml_ugreatereq:
        case iml_jmpneq:
        case iml_jmpuless:
        case iml_jmpugreatereq:
        case iml_call:
        case iml_call_c:
        case iml_set:
        case iml_get:
            op->arg1 = va_arg(argp, void *);
            op->arg2 = va_arg(argp, void *);
            op->arg3 = va_arg(argp, void *);
            break;
        case iml_setelm:
            op->arg1 = va_arg(argp, void *);
            op->arg2 = va_arg(argp, void *);
            op->arg3 = va_arg(argp, void *);
            op->arg4 = GUINT_TO_POINTER(va_arg(argp, guint));
            break;
        case iml_getelm:
            op->arg1 = va_arg(argp, void *);
            op->arg2 = va_arg(argp, void *);
            op->arg3 = GUINT_TO_POINTER(va_arg(argp, guint));
            op->arg4 = va_arg(argp, void *);
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }
    va_end(argp);

    return op;
}

iml_operation_t *
iml_operation_new_call_c(gchar *function, ImlVariable *res, ...)
{
    GSList *arguments = NULL;
    void *arg;
    va_list argp;

    va_start(argp, res);
    while ((arg = va_arg(argp, void *)) != NULL)
    {
        assert(IML_IS_OPERAND(arg));
        arguments = g_slist_prepend(arguments, arg);
    }
    arguments = g_slist_reverse(arguments);
    va_end(argp);

    return iml_operation_new(iml_call_c, function, arguments, res);
}

iml_opcode_t
iml_operation_get_opcode(iml_operation_t *self)
{
	assert(self);

	return self->opcode;
}

void *
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
        case 4:
            return self->arg4;
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
        case iml_return:
            print_return_op(self, out, indention);
            break;
        case iml_copy:
        case iml_cast:
        case iml_ineg:
        case iml_bneg:
        case iml_getaddr:
            print_binary_op(self, out, indention);
            break;
        case iml_add:
        case iml_sub:
        case iml_smult:
        case iml_umult:
        case iml_sdiv:
        case iml_udiv:
        case iml_smod:
        case iml_umod:
        case iml_and:
        case iml_or:
        case iml_equal:
        case iml_nequal:
        case iml_sless:
        case iml_uless:
        case iml_sgreater:
        case iml_ugreater:
        case iml_slesseq:
        case iml_ulesseq:
        case iml_sgreatereq:
        case iml_ugreatereq:
            print_ternary_op(self, out, indention);
            break;
        case iml_mset:
            print_mset_op(self, out, indention);
            break;
        case iml_set:
            print_set_op(self, out, indention);
            break;
        case iml_get:
            print_get_op(self, out, indention);
            break;
        case iml_setelm:
            print_setelm_op(self, out, indention);
            break;
        case iml_getelm:
            print_getelm_op(self, out, indention);
            break;
        case iml_jmp:
            fprintf_indent(out, indention, "jmp %s\n", self->arg1);
            break;
        case iml_jmpneq:
        case iml_jmpuless:
        case iml_jmpugreatereq:
            print_jmpcond_op(self, out, indention);
            break;
        case iml_call:
        case iml_call_c:
            print_call_op(self, out, indention);
            break;
        case iml_label:
            fprintf_indent(out, indention, "%s:\n", self->arg1);
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
print_binary_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);

    gchar *op_name;

    switch (op->opcode)
    {
        case iml_copy:
            op_name = "copy";
            break;
        case iml_cast:
            op_name = "cast";
            break;
        case iml_ineg:
            op_name = "ineg";
            break;
        case iml_bneg:
            op_name = "bneg";
            break;
        case iml_getaddr:
            op_name = "getaddr";
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }

    fprintf_indent(out, indention, "%s ", op_name);
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, " => ");
    iml_operand_print_short(op->arg2, out, 0);
    fprintf(out, "\n");
}

static void
print_ternary_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);

    gchar *op_name;

    switch (op->opcode)
    {
        case iml_add:
            op_name = "add";
            break;
        case iml_sub:
            op_name = "sub";
            break;
        case iml_smult:
            op_name = "smult";
            break;
        case iml_umult:
            op_name = "umult";
            break;
        case iml_sdiv:
            op_name = "sdiv";
            break;
        case iml_udiv:
            op_name = "udiv";
            break;
        case iml_smod:
            op_name = "smod";
            break;
        case iml_umod:
            op_name = "umod";
            break;
        case iml_and:
            op_name = "and";
            break;
        case iml_or:
            op_name = "or";
            break;
        case iml_equal:
            op_name = "equal";
            break;
        case iml_nequal:
            op_name = "nequal";
            break;
        case iml_sless:
            op_name = "sless";
            break;
        case iml_uless:
            op_name = "uless";
            break;
        case iml_sgreater:
            op_name = "sgreater";
            break;
        case iml_ugreater:
            op_name = "ugreater";
            break;
        case iml_slesseq:
            op_name = "slesseq";
            break;
        case iml_ulesseq:
            op_name = "ulesseq";
            break;
        case iml_sgreatereq:
            op_name = "sgreatereq";
            break;
        case iml_ugreatereq:
            op_name = "ugreatereq";
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }

    fprintf_indent(out, indention, "%s ", op_name);
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, ", ");
    iml_operand_print_short(op->arg2, out, 0);
    fprintf(out, " => ");
    iml_operand_print_short(op->arg3, out, 0);
    fprintf(out, "\n");
}

static void
print_mset_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);
    assert(op->opcode == iml_mset);

    fprintf_indent(out, indention, "mset ");
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, ", %u => ", GPOINTER_TO_UINT(op->arg2));
    iml_operand_print_short(op->arg3, out, 0);
    fprintf(out, "\n");
}

static void
print_set_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);
    assert(op->opcode == iml_set);

    fprintf_indent(out, indention, "set ");
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, " => [");
    iml_operand_print_short(op->arg2, out, 0);
    if (op->arg3 != NULL)
    {
        fprintf(out, " + ");
        iml_operand_print_short(op->arg3, out, 0);
    }
    fprintf(out, "]\n");
}

static void
print_get_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);
    assert(op->opcode == iml_get);

    fprintf_indent(out, indention, "get [");
    iml_operand_print_short(op->arg1, out, 0);
    if (op->arg2 != NULL)
    {
        fprintf(out, " + ");
        iml_operand_print_short(op->arg2, out, 0);
    }
    fprintf(out, "] => ");
    iml_operand_print_short(op->arg3, out, 0);
    fprintf(out, "\n");
}

static void
print_setelm_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);
    assert(op->opcode == iml_setelm);

    fprintf_indent(out, indention, "setelm ");
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, " => ");
    iml_operand_print_short(op->arg2, out, 0);
    fprintf(out, "(");
    iml_operand_print_short(op->arg3, out, 0);
    fprintf(out, "*%u)\n", GPOINTER_TO_UINT(op->arg4));
}

static void
print_getelm_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);
    assert(op->opcode == iml_getelm);

    fprintf_indent(out, indention, "getelm ");
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, "(");
    iml_operand_print_short(op->arg2, out, 0);
    fprintf(out, "*%u) => ", GPOINTER_TO_UINT(op->arg3));
    iml_operand_print_short(op->arg4, out, 0);
    fprintf(out, "\n");
}

static void
print_return_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);
    assert(op->opcode == iml_return);

    fprintf_indent(out, indention, "return ");
    if (op->arg1 != NULL)
    {
        iml_operand_print_short(op->arg1, out, 0);
    }
    fprintf(out, "\n");
}

static void
print_jmpcond_op(iml_operation_t *op, FILE *out, int indention)
{
    assert(op);

    gchar *op_name;

    switch (op->opcode)
    {
        case iml_jmpneq:
            op_name = "jmpneq";
            break;
        case iml_jmpuless:
            op_name = "jmpuless";
            break;
        case iml_jmpugreatereq:
            op_name = "jmpugreatereq";
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }

    fprintf_indent(out, indention, "%s ", op_name);
    iml_operand_print_short(op->arg1, out, 0);
    fprintf(out, ",");
    iml_operand_print_short(op->arg2, out, 0);
    fprintf(out, " => %s\n", (char*)op->arg3);
}

static void
print_call_op(iml_operation_t *op, FILE *out, int indention)
{
    GSList *i;
    assert(op);
    assert(op->opcode == iml_call || op->opcode == iml_call_c);

    fprintf_indent(out, indention, "call%s %s (",
                   op->opcode == iml_call ? "" : "_c",
                   op->arg1);

    for (i = (GSList *)op->arg2; i != NULL; i = g_slist_next(i))
    {
        iml_operand_print_short(i->data, out, 0);
        if (g_slist_next(i) != NULL)
        {
            fprintf(out, ", ");
        }
    }
    fprintf(out, ") ");

    if (op->arg3 != NULL)
    {
        fprintf(out, "=> ");
        iml_operand_print_short(op->arg3, out, 0);
    }

    fprintf(out, "\n");
}
