JAVA_TRGT_OBJS := java_trgt.o java_local_slot.o
X86_TRGT_OBJS := x86.o x86_cast.o x86_reg_location.o x86_frame_offset.o

OBJS := auxil.o entire.o lex.o yygrammar.o sym_table.o utils.o types.o        \
	sem_analyze.o label_gen.o ast_array_constant.o ast_while.o            \
        ast_node.o ast_compile_unit.o ast_function.o ast_data_type.o          \
        ast_basic_type.o ast_code_block.o ast_statment.o ast_expression.o     \
        ast_return.o ast_constant.o ast_int_constant.o ast_bool_constant.o    \
        ast_binary_operation.o ast_unary_operation.o ast_assigment.o          \
        ast_variable_declaration.o ast_variable_definition.o ast_if_block.o   \
        ast_variable_ref.o ast_function_call.o ast_if_else.o ast_foreach.o    \
        ast_static_array_type.o ast_array_cell_ref.o ast_array_slice_ref.o    \
        ast_auto_type.o                                                       \
        ir_symbol.o ir_variable.o ir_function.o ir_code_block.o ir_statment.o \
        ir_int_constant.o ir_binary_operation.o ir_unary_operation.o          \
        ir_bool_constant.o ir_cast.o ir_compile_unit.o ir_if_else.o           \
        ir_if_block.o ir_while.o ir_foreach.o ir_expression.o ir_return.o     \
        ir_function_call.o ir_assigment.o                                     \
        $(X86_TRGT_OBJS) $(JAVA_TRGT_OBJS)
