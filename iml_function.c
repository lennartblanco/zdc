#include <glib.h>
#include <stdbool.h>

#include "iml_function.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_function_s
{
    IrModule *parent_module;
    ir_linkage_type_t linkage;
    const char *name;
    GSList *operations;
    GSList *parameters;
    GSList *loc_vars;
    GHashTable *unused_temp_vars;
    GSList *used_regs;
    guint stack_size;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_function_t *
cast_iml_function(void *obj)
{
    return (iml_function_t *)obj;
}

iml_function_t *
iml_function_new(IrModule *parent_module, ir_linkage_type_t linkage)
{
    iml_function_t *obj;

    obj = g_malloc0(sizeof(*obj));

    obj->parent_module = parent_module;
    obj->linkage = linkage;
    obj->name = NULL;
    obj->unused_temp_vars = g_hash_table_new(g_direct_hash, g_direct_equal);

    return obj;
}

IrModule *
iml_function_get_parent_module(iml_function_t *self)
{
    assert(self);

    return self->parent_module;
}

ir_linkage_type_t
iml_function_get_linkage(iml_function_t *self)
{
    assert(self);

    return self->linkage;
}

void
iml_function_set_name(iml_function_t *self, const char *name)
{
    assert(self);
    assert(name);

    self->name = name;
}

const char *
iml_function_get_name(iml_function_t *self)
{
    assert(self);
    assert(self->name);

    return self->name;
}

void
iml_function_add_operation(iml_function_t *self, iml_operation_t *operation)
{
    assert(self);
    assert(operation);

    self->operations = g_slist_append(self->operations, operation);
}

GSList *
iml_function_get_operations(iml_function_t *self)
{
    assert(self);

    return self->operations;
}

void
iml_function_add_local(iml_function_t *self, ImlVariable *variable)
{
    assert(self);
    assert(iml_is_variable(variable));

    self->loc_vars = g_slist_prepend(self->loc_vars, variable);
}

GSList *
iml_function_get_locals(iml_function_t *self)
{
    assert(self);

    return self->loc_vars;
}

void
iml_function_add_parameter(iml_function_t *self, ImlVariable *variable)
{
    assert(self);
    assert(variable);

    self->parameters = g_slist_append(self->parameters, variable);
}

GSList *
iml_function_get_parameters(iml_function_t *self)
{
  assert(self);

  return self->parameters;
}

ImlVariable *
iml_function_get_temp(iml_function_t *self, iml_data_type_t datatype, ...)
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

        iml_function_add_local(self, temp_var);
    }

    return temp_var;
}

void
iml_function_unused_oper(iml_function_t *self, ImlOperand *oper)
{
    assert(self);

    GSList *vars;
    iml_data_type_t var_type;
    ImlVariable *temp_var;

    if (!iml_is_variable(oper) ||
        !iml_variable_is_temp(iml_variable(oper)))
    {
        /* not a temporary variable, do nothing */
        return;
    }

    temp_var = iml_variable(oper);

    var_type = iml_variable_get_data_type(temp_var);
    vars = g_hash_table_lookup(self->unused_temp_vars,
                               GINT_TO_POINTER(var_type));
    vars = g_slist_append(vars, temp_var);
    g_hash_table_insert(self->unused_temp_vars,
                        GINT_TO_POINTER(var_type),
                        vars);
}

void
iml_function_set_frame_size(iml_function_t *self, guint stack_size)
{
	assert(self);

	self->stack_size = stack_size;
}

guint
iml_function_get_frame_size(iml_function_t *self)
{
	assert(self);

	return self->stack_size;
}

void
iml_function_set_used_regs(iml_function_t *self,
                             GSList *preserved_regs)
{
    assert(self);

    self->used_regs = preserved_regs;
}

GSList *
iml_function_get_used_regs(iml_function_t *self)
{
    assert(self);

    return self->used_regs;
}

void
iml_function_print(iml_function_t *self, FILE *out, int indention)
{
    GSList *i;

    fprintf_indent(out, indention,
                   "function: '%s'\n  frame:\n    stack size %u\n",
                   self->name, self->stack_size);

    /*
     * print function frame
     */
    if (self->parameters != NULL)
    {
        fprintf_indent(out, indention + 4, "parameters\n");
        for (i = self->parameters; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 6);
            fprintf(out, "\n");
        }
    }

    if (self->loc_vars != NULL)
    {
        fprintf_indent(out, indention + 4, "variables\n");

        for (i = self->loc_vars; i != NULL; i = g_slist_next(i))
        {
            iml_operand_print(i->data, out, indention + 6);
            fprintf(out, "\n");
        }
    }

    /*
     * print function body
     */
    fprintf_indent(out, indention + 2, "body:\n");
    for (i = self->operations; i != NULL; i = g_slist_next(i))
    {
        iml_operation_print(i->data, out, indention + 4);
    }
}
