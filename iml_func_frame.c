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
    GSList *loc_vars;
    GHashTable *unused_temp_vars;
    GSList *used_regs;
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
    frame->unused_temp_vars = g_hash_table_new(g_direct_hash, g_direct_equal);

    return frame;
}

void
iml_func_frame_add_local(iml_func_frame_t *self, ImlVariable *variable)
{
    assert(self);
    assert(iml_is_variable(variable));

    self->loc_vars = g_slist_prepend(self->loc_vars, variable);
}

GSList *
iml_func_frame_get_locals(iml_func_frame_t *self)
{
    assert(self);

    return self->loc_vars;
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
iml_func_frame_get_temp(iml_func_frame_t *self, iml_data_type_t datatype, ...)
{
    ImlVariable *temp_var = NULL;
    GSList *vars;
    guint blob_size;

    if (datatype == iml_blob)
    {
        va_list argp;

        va_start(argp, datatype);
        blob_size = va_arg(argp, guint);
        va_end(argp);
    }

    /*
     * check if we can reuse already existing, but currently unused variable
     */
    vars = g_hash_table_lookup(self->unused_temp_vars,
                               GINT_TO_POINTER(datatype));
    if (vars != NULL)
    {
        temp_var = vars->data;
        if (datatype == iml_blob)
        {
            /* only temporary blob variables of same size supported for now */
            assert(iml_variable_get_size(temp_var) == blob_size);
        }

        /* remove the variable from unused list */
        vars = g_slist_remove(vars, temp_var);
        g_hash_table_insert(self->unused_temp_vars,
                            GINT_TO_POINTER(datatype),
                            vars);
    }

    /*
     * create new temporary variable if needed
     */
    if (temp_var == NULL)
    {
        if (datatype == iml_blob)
        {
            temp_var = iml_variable_blob_new(blob_size, NULL);
        }
        else
        {
            temp_var = iml_variable_new(datatype, NULL);
        }

        iml_func_frame_add_local(self, temp_var);
    }

    return temp_var;
}

void
iml_func_frame_unused_oper(iml_func_frame_t *self, ImlOperand *oper)
{
    assert(self);

    GSList *vars;
    iml_data_type_t var_type;
    ImlVariable *temp_var;

    if (!iml_is_variable(oper) ||
        !iml_variable_is_temp(IML_VARIABLE(oper)))
    {
        /* not a temporary variable, do nothing */
        return;
    }

    temp_var = IML_VARIABLE(oper);

    var_type = iml_variable_get_data_type(temp_var);
    vars = g_hash_table_lookup(self->unused_temp_vars,
                               GINT_TO_POINTER(var_type));
    vars = g_slist_append(vars, temp_var);
    g_hash_table_insert(self->unused_temp_vars,
                        GINT_TO_POINTER(var_type),
                        vars);
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
iml_func_frame_set_used_regs(iml_func_frame_t *self,
                             GSList *preserved_regs)
{
    assert(self);

    self->used_regs = preserved_regs;
}

GSList *
iml_func_frame_get_used_regs(iml_func_frame_t *self)
{
    assert(self);

    return self->used_regs;
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

    if (self->loc_vars != NULL)
    {
        fprintf_indent(out, indention + 2, "variables\n");

        for (i = self->loc_vars; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 4);
            fprintf(out, "\n");
        }
    }
}
