#include "ast_basic_type.h"
#include "ir_variable.h"
#include "ir_function.h"

int main()
{
    IrVariable *var;
    IrFunction *func;

    g_type_init();

    /* int hej; */
    var = 
        ir_variable_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "hej");
    printf("var name = '%s'\n",
           ir_symbol_get_name(IR_SYMBOL(var)));
                  

    return 0;
}
