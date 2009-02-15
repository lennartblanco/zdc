EXAMPLES = ast_example1 ast_example2 ir_example
CFLAGS  += -I.. -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0 gobject-2.0)

all: $(EXAMPLES)

ast_example1:
	gcc $(CFLAGS) -o ast_example1 ast_example1.c                        \
            ../ast_code_block.c ../ast_node.c ../ast_compile_unit.c         \
            ../ast_function.c ../ast_basic_type.c ../ast_data_type.c        \
            ../ast_variable_declaration.c ../ast_statment.c ../ast_return.c \
            ../ast_bool_constant.c ../ast_constant.c ../ast_expression.c    \
            ../ast_function_call.c ../ast_int_constant.c ../ast_assigment.c \
            ../ast_unary_operation.c ../ast_binary_operation.c              \
            ../ast_variable_definition.c ../ast_variable_ref.c              \
            $(LDFLAGS)

ast_example2:
	gcc $(CFLAGS) -o ast_example2 ast_example2.c                        \
            ../ast_variable_ref.c ../ast_expression.c ../ast_int_constant.c \
            ../ast_array_cell_ref.c ../ast_statment.c  ../ast_constant.c    \
            ../ast_node.c                                                   \
            $(LDFLAGS)

ir_example:
	gcc $(CFLAGS) -o ir_example ir_example.c                            \
            ../ast_basic_type.c ../ast_data_type.c ../ast_node.c \
            ../ir_variable.c ../ir_symbol.c \
            $(LDFLAGS)


clean:
	rm -f $(EXAMPLES) *~
