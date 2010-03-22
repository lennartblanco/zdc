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
    IrCodeBlock  *body;
    char *mangled_name;
};


/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_function_def_class_init(gpointer klass, gpointer foo);

static void
ir_function_def_do_print(IrSymbol *self, FILE *out, int indention);

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
        ir_function_def_class_init,  /* class_init */
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
ir_function_def_new(DtDataType *return_type,
                    char *name,
                    GSList *parameters,
                    IrModule *parent_module,
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
                       "ir-symbol-parent-module", parent_module,
                       NULL);

    obj->mangled_name = NULL;
    
    ir_function_set_return_type(IR_FUNCTION(obj), return_type);
    ir_function_set_parameters(IR_FUNCTION(obj), parameters);

    /*
     * set-up the symbol table containing function's parameters
     */
    obj->param_symbols = sym_table_new(ir_module_get_symbols(parent_module));
    p = parameters;
    for (; p != NULL; p = g_slist_next(p))
    {
        /* check if this is a named parameter */
        if (IR_IS_VARIABLE(p->data))
        {
          /* add it to function's symbol table */
          sym_table_add_symbol(obj->param_symbols, IR_SYMBOL(p->data));
        }
    }

    obj->body = ir_code_block_new(obj->param_symbols);
    
    return obj;
}

char *
ir_function_def_get_name(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_name(IR_FUNCTION(self));
}

char *
ir_function_def_get_mangled_name(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    if (self->mangled_name == NULL)
    {
        GSList *i;
        char *func_name;

        GString *str =
            g_string_new(
                ir_module_get_mangled_name(
                    ir_symbol_get_parent_module(IR_SYMBOL(self))));

        func_name = ir_function_get_name(IR_FUNCTION(self));
        g_string_append_printf(str, "%zu%sF", strlen(func_name), func_name);

        i = ir_function_def_get_parameters(self);
        for (; i != NULL; i = g_slist_next(i))
        {
            DtDataType *var_type;

            if (IR_IS_VARIABLE(i->data))
            {
                var_type = ir_variable_get_data_type(i->data);
            }
            else
            {
                /* unnamed function parameter, only the type specified */
                assert(DT_IS_DATA_TYPE(i->data));
                var_type = i->data;
            }

            g_string_append(str, dt_data_type_get_mangled(var_type));
        }

        g_string_append_printf(
            str, "Z%s",
            dt_data_type_get_mangled(ir_function_def_get_return_type(self)));


        self->mangled_name = g_string_free(str, FALSE);
    }

    return self->mangled_name;
}

GSList *
ir_function_def_get_parameters(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_parameters(IR_FUNCTION(self));
}

DtDataType *
ir_function_def_get_return_type(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_return_type(IR_FUNCTION(self));
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

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_function_def_class_init(gpointer klass, gpointer foo)
{
    ((IrSymbolClass *)klass)->do_print = ir_function_def_do_print;
}

static void
ir_function_def_do_print(IrSymbol *self, FILE *out, int indention)
{
    assert(IR_IS_FUNCTION_DEF(self));
    assert(out);

    IrFunctionDef *func;

    ir_function_print(IR_FUNCTION(self), out, indention);

    func = IR_FUNCTION_DEF(self);
    fprintf_indent(out, indention + 2, "body:\n");
    ir_statment_print(IR_STATMENT(func->body), out, indention + 2);

}

