#include <stdio.h>

#include "ast_array_cell_ref.h"
#include "ast_int_constant.h"

int
main()
{
    AstVariableRef       *var_ref;
    AstArrayCellRef      *acell_ref;
    AstExpression        *exp;

    g_type_init();

    
    /* a reference to the value of variable 'foo' */
    var_ref = ast_variable_ref_new("foo");
    
    /* a reference to the value of 'bar[2]' */
    exp = XDP_AST_EXPRESSION(ast_int_constant_new(2));
    acell_ref = ast_array_cell_ref_new("bar", exp);

    /* get name for scalar variable ref and array cell ref */
    printf("scalar variable reference name: '%s'\n",
           ast_variable_ref_get_name(XDP_AST_VARIABLE_REF(var_ref)));

    printf("scalar array cell reference name: '%s'\n",
           ast_variable_ref_get_name(XDP_AST_VARIABLE_REF(acell_ref)));

    return 0;
}
