#include "ir_function.h"
#include "ir_variable.h"
#include "ast_variable_declaration.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_function_class_init(gpointer klass, gpointer foo);

static void
ir_function_do_print(IrSymbol *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrFunctionClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_function_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFunction),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "IrFunctionType",
                                    &info, 0);
    }
    return type;
}

IrFunction *
ir_function_new(AstDataType *return_type,
                char *name,
                GSList *parameters,
                sym_table_t *parent_scope)
{
    assert(return_type);
    assert(name);
    assert(parent_scope);

    IrFunction *obj;
    GSList *p;

    obj = g_object_new(IR_TYPE_FUNCTION,
                       "ir-symbol-name", name,
                       NULL);

    obj->return_type = return_type;
    obj->parameters = parameters;

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
ir_function_get_name(IrFunction *self)
{
    assert(self);
    assert(IR_IS_FUNCTION(self));

    return ir_symbol_get_name(IR_SYMBOL(self));
}

GSList *
ir_function_get_parameters(IrFunction *self)
{
    assert(self);
    assert(IR_IS_FUNCTION(self));

    return self->parameters;
}

sym_table_t *
ir_function_get_parameter_symbols(IrFunction *self)
{
    assert(self);
    assert(IR_IS_FUNCTION(self));

    return self->param_symbols;
}

AstDataType *
ir_function_get_return_type(IrFunction *self)
{
    assert(self);
    assert(IR_IS_FUNCTION(self));

    return self->return_type;
}

IrCodeBlock *
ir_function_get_body(IrFunction *self)
{
    assert(self);
    assert(IR_IS_FUNCTION(self));

    return self->body;
}
/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_function_do_print(IrSymbol *self, FILE *out, int indention)
{
    assert(self);
    assert(out);

    IrFunction *func = IR_FUNCTION(self);

    fprintf_indent(out, indention,
            "function [%p]\n"
            "  name: '%s'\n"
            "  return_type: ",
            self,
            ir_symbol_get_name(self));

    ast_node_print(XDP_AST_NODE(func->return_type), out);
    fprintf_indent(out, indention, "\n  parameters: ");

    GSList *i = func->parameters;
    for (;i != NULL; i = g_slist_next(i))
    {
        ast_node_print(XDP_AST_NODE(i->data), out);
        fprintf(out, "%s", g_slist_next(i) != NULL ? ", " : "");
    }
    fprintf_indent(out, indention, "\n  body:\n");
    ir_code_block_print(func->body, out, indention + 2);
}

static void
ir_function_class_init(gpointer klass, gpointer foo)
{
    ((IrSymbolClass *)klass)->do_print = ir_function_do_print;
}
