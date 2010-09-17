#include <glib.h>
#include <stdbool.h>

#include "iml_func_frame.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_func_frame_s
{
    GSList *parameters;
    GSList *var_32b;
    GSList *var_16b;
    GSList *var_8b;
    guint stack_size;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_func_frame_t *
iml_func_frame_new(void)
{
    iml_func_frame_t *frame;

    frame = g_malloc0(sizeof(*frame));

    return frame;
}

void
iml_func_frame_add_local(iml_func_frame_t *self, ImlVariable *variable)
{
    assert(self);
    assert(variable);

    switch (iml_variable_get_data_type(variable))
    {
        case iml_8b:
            self->var_8b = g_slist_prepend(self->var_8b, variable);
            break;
        case iml_16b:
            self->var_16b = g_slist_prepend(self->var_16b, variable);
            break;
        case iml_32b:
            self->var_32b = g_slist_prepend(self->var_32b, variable);
            break;
        default:
            /* unexpected data type */
            assert(false);
    }
}

GSList *
iml_func_frame_get_locals(iml_func_frame_t *self,
                          iml_data_type_t vars_type)
{
    assert(self);

    switch (vars_type)
    {
        case iml_32b:
            return self->var_32b;
        case iml_16b:
            return self->var_16b;
        case iml_8b:
            return self->var_8b;
    }
    /* unexpected local variables data type */
    assert(FALSE);
}

void
iml_func_frame_add_parameter(iml_func_frame_t *self, ImlVariable *variable)
{
    assert(self);
    assert(variable);

    self->parameters = g_slist_append(self->parameters, variable);
}

GSList *
iml_func_frame_get_parameters(iml_func_frame_t *self)
{
  assert(self);

  return self->parameters;
}

ImlVariable *
iml_func_frame_get_temp(iml_func_frame_t *self, iml_data_type_t datatype)
{
    ImlVariable *temp_var = iml_variable_new(datatype, NULL);

    iml_func_frame_add_local(self, temp_var);

    return temp_var;
}

void
iml_func_frame_set_size(iml_func_frame_t *self, guint stack_size)
{
	assert(self);

	self->stack_size = stack_size;
}

guint
iml_func_frame_get_size(iml_func_frame_t *self)
{
	assert(self);

	return self->stack_size;
}


void
iml_func_frame_print(iml_func_frame_t *self, FILE *out, int indention)
{
    GSList *i;

    fprintf_indent(out, indention + 2, "stack size %u\n", self->stack_size);

    if (self->parameters != NULL)
    {
        fprintf_indent(out, indention + 2, "parameters\n");
        for (i = self->parameters; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 4);
            fprintf(out, "\n");
        }
    }

    if (self->var_32b || self->var_16b || self->var_8b)
    {
        fprintf_indent(out, indention + 2, "variables\n");

        for (i = self->var_32b; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 4);
            fprintf(out, "\n");
        }

        for (i = self->var_16b; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 4);
            fprintf(out, "\n");
        }

        for (i = self->var_8b; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 4);
            fprintf(out, "\n");
        }
    }
}
