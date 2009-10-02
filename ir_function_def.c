#include "ir_function_def.h"
#include "ast_variable_declaration.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

//static void
//ir_function_def_class_init(gpointer klass, gpointer foo);

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
ir_function_def_new(AstDataType *return_type,
                    char *name,
                    GSList *parameters,
                    sym_table_t *parent_scope)
{
    assert(XDP_IS_AST_DATA_TYPE(return_type));
    assert(name);
    assert(parent_scope);

    IrFunctionDef *obj;
    GSList *p;

    obj = g_object_new(IR_TYPE_FUNCTION_DEF,
                       "ir-symbol-name", name,
                       NULL);

    
    ir_function_set_return_type(IR_FUNCTION(obj), return_type);
//    obj->return_type = return_type;
    ir_function_set_parameters(IR_FUNCTION(obj), parameters);
//    obj->parameters = parameters;

    /*
     * set-up the symbol table containing function's parameters
     */
    obj->param_symbols = sym_table_new(parent_scope);
    p = parameters;
    for (; p != NULL; p = g_slist_next(p))
    {        
        AstVariableDeclaration *var_decl = p->data;

        /* convert ast variable declaration to IR variable object */
        IrVariable *variable = 
            ir_variable_new(ast_variable_declaration_get_data_type(var_decl),
                            ast_variable_declaration_get_name(var_decl), NULL);

        /* add to parameter to function's symbol table */
        sym_table_add_symbol(obj->param_symbols, IR_SYMBOL(variable));
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

GSList *
ir_function_def_get_parameters(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_parameters(IR_FUNCTION(self));
}

AstDataType *
ir_function_def_get_return_type(IrFunctionDef *self)
{
    assert(IR_IS_FUNCTION_DEF(self));

    return ir_function_get_return_type(IR_FUNCTION(self));
}

sym_table_t *
ir_function_def_get_parameter_symbols(IrFunctionDef *self)
{
    assert(self);
    assert(IR_IS_FUNCTION_DEF(self));

    return self->param_symbols;
}

IrCodeBlock *
ir_function_def_get_body(IrFunctionDef *self)
{
    assert(self);
    assert(IR_IS_FUNCTION_DEF(self));

    return self->body;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

//static void
//ir_function_class_init(gpointer klass, gpointer foo)
//{
//    ((IrSymbolClass *)klass)->do_print = ir_function_do_print;
//}
