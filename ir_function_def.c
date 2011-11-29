#include <string.h>

#include "ir_function_def.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct _IrFunctionDef
{
    IrFunction parent;
    /* private */
    sym_table_t  *param_symbols;
    iml_func_frame_t *frame;
    GSList *operations;
    IrCodeBlock  *body;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_def_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrFunctionDefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFunctionDef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_FUNCTION,
                                    "IrFunctionDefType",
                                    &info, 0);
    }
    return type;
}

IrFunctionDef *
ir_function_def(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj),
                                      IR_TYPE_FUNCTION_DEF,
                                      IrFunctionDef);
}

IrFunctionDef *
ir_function_def_new(DtDataType *return_type,
                    char *name,
                    GSList *parameters,
                    IrModule *parent_module,
                    ir_linkage_type_t linkage_type,
                    guint line_number)
{
    assert(DT_IS_DATA_TYPE(return_type));
    assert(name);
    assert(IR_IS_MODULE(parent_module));

    IrFunctionDef *obj;
    GSList *p;

    obj = g_object_new(IR_TYPE_FUNCTION_DEF,
                       "ir-node-line-number", line_number,
                       "ir-symbol-name", name,
                       "ir-function-linkage-type", linkage_type,
                       "ir-symbol-parent-module", parent_module,
                       NULL);

    obj->frame = iml_func_frame_new();
    obj->operations = NULL;
    
    ir_function_set_return_type(ir_function(obj), return_type);
    ir_function_set_parameters(ir_function(obj), parameters);

    /*
     * set-up the symbol table containing function's parameters
     */
    obj->param_symbols = sym_table_new(ir_module_get_symbols(parent_module));
    p = parameters;
    for (; p != NULL; p = g_slist_next(p))
    {
        /* check if this is a named parameter */
        if (ir_variable_get_name(ir_variable(p->data)) != NULL)
        {
          /* add it to function's symbol table */
          sym_table_add_symbol(obj->param_symbols, ir_symbol(p->data));
        }
    }

    obj->body = ir_code_block_new(obj->param_symbols);
    
    return obj;
}

char *
ir_function_def_get_name(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_name(ir_function(self));
}

GSList *
ir_function_def_get_parameters(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_parameters(ir_function(self));
}

void
ir_function_def_set_return_type(IrFunctionDef *self, DtDataType *type)
{
    assert(IR_IS_FUNCTION_DEF(self));
    assert(DT_IS_DATA_TYPE(type));

    ir_function_set_return_type(ir_function(self), type);
}

DtDataType *
ir_function_def_get_return_type(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_return_type(ir_function(self));
}

sym_table_t *
ir_function_def_get_parameter_symbols(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return self->param_symbols;
}

IrCodeBlock *
ir_function_def_get_body(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return self->body;
}

iml_func_frame_t *
ir_function_def_get_frame(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return self->frame;
}

void
ir_function_def_add_operation(IrFunctionDef *self, iml_operation_t *operation)
{
    assert(IR_IS_FUNCTION_DEF(self));
    assert(operation);

    self->operations = g_slist_append(self->operations, operation);
}

GSList *
ir_function_def_get_operations(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return self->operations;
}
