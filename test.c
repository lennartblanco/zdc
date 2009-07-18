#include "ast_basic_type.h"
#include "ir_int_constant.h"
#include "ir_variable.h"

int
main()
{
  IrIntConstant *int_const;
  IrVariable *int_var;
  IrVariable *bool_var;

  AstDataType *data_type;

  g_type_init();

  int_const = ir_int_constant_new(16);

  data_type = ir_expression_get_data_type(IR_EXPRESSION(int_const));

  printf("int_const data type '");
  ast_node_print(XDP_AST_NODE(data_type), stdout);
  printf("'\n");

  int_var = ir_variable_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                            "int_variable", NULL);

  data_type = ir_expression_get_data_type(IR_EXPRESSION(int_var));
  printf("int_var data type '");
  ast_node_print(XDP_AST_NODE(data_type), stdout);
  printf("'\n");

  bool_var = ir_variable_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                             "boolean_variable", NULL);

  data_type = ir_expression_get_data_type(IR_EXPRESSION(bool_var));
  printf("bool_var data type '");
  ast_node_print(XDP_AST_NODE(data_type), stdout);
  printf("'\n");

  return 0;
}

