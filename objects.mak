JAVA_TRGT_OBJS := java_trgt.o java_local_slot.o

X86_TRGT_OBJS := x86.o x86_cast.o x86_reg_location.o x86_frame_offset.o       \
        x86_if_else.o x86_func_call.o x86_code_block.o x86_while.o            \
        x86_foreach.o

OBJS := auxil.o entire.o parser.o lex.o yygrammar.o sym_table.o utils.o       \
        types.o types_arrays.o                                                \
        ast_to_ir.o sem_analyze.o sem_analyze_validate.o label_gen.o          \
        ast_array_literal.o ast_while.o ast_code_block.o ast_statment.o       \
        ast_node.o ast_module.o ast_function_def.o ast_function_decl.o        \
        ast_expression.o ast_return.o ast_constant.o ast_int_constant.o       \
        ast_uint_constant.o ast_bool_constant.o                               \
        ast_binary_operation.o ast_unary_operation.o                          \
        ast_assigment.o ast_variable_declaration.o ast_variable_definition.o  \
        ast_if_block.o ast_variable_ref.o ast_function_call.o ast_if_else.o   \
        ast_foreach.o ast_array_cell_ref.o ast_array_slice_ref.o              \
        dt_data_type.o dt_auto_type.o dt_basic_type.o dt_array_type.o         \
        dt_static_array_type.o                                                \
        ir_node.o ir_symbol.o ir_variable.o ir_function.o ir_code_block.o     \
        ir_statment.o ir_cast.o ir_bool_constant.o ir_int_constant.o          \
        ir_uint_constant.o ir_binary_operation.o ir_unary_operation.o         \
        ir_module.o ir_if_else.o ir_if_block.o ir_while.o ir_foreach.o        \
        ir_expression.o ir_return.o ir_function_call.o ir_assigment.o         \
        ir_array_literal.o ir_lvalue.o ir_array_slice.o                       \
        ir_array_cell.o ir_scalar.o ir_function_def.o ir_function_decl.o      \
        $(X86_TRGT_OBJS) #$(JAVA_TRGT_OBJS)
