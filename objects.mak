X86_TRGT_OBJS := x86.o x86_data.o

ARM_TRGT_OBJS := arm-d.o

# objects generated from C source files
COBJS := auxil.o entire.o parser.o lex.o yygrammar.o sym_table.o utils.o      \
         errors.o types.o types_arrays.o const_fold.o ir_to_iml.o             \
         ast_to_ir.o sem_analyze.o sem_analyze_validate.o                     \
         ast_array_literal.o ast_while.o ast_code_block.o ast_statment.o      \
         ast_node.o ast_module.o ast_function_def.o ast_function_decl.o       \
         ast_expression.o ast_return.o ast_constant.o ast_int_constant.o      \
         ast_uint_constant.o ast_char_constant.o ast_bool_constant.o          \
         ast_import.o ast_binary_operation.o ast_unary_operation.o            \
         ast_postfix_exp.o ast_assigment.o ast_variable_declaration.o         \
         ast_variable_definition.o  ast_if_block.o ast_ident.o                \
         ast_function_call.o ast_if_else.o ast_foreach.o ast_array_cell_ref.o \
         ast_array_slice_ref.o ast_string_literal.o ast_alias.o ast_enum.o    \
         ast_enum_member.o                                                    \
         dt_data_type.o dt_auto_type.o dt_basic_type.o dt_array_type.o        \
         dt_static_array_type.o dt_user_type.o dt_enum_type.o                 \
         ir_node.o ir_symbol.o ir_variable.o ir_function.o ir_enum_member.o   \
         ir_code_block.o ir_statment.o ir_cast.o ir_constant.o ir_enum.o      \
         ir_null.o ir_bool_constant.o ir_int_constant.o ir_uint_constant.o    \
         ir_char_constant.o ir_binary_operation.o ir_unary_operation.o        \
         ir_module.o ir_if_else.o ir_if_block.o ir_while.o ir_foreach.o       \
         ir_expression.o ir_return.o ir_function_call.o ir_assigment.o        \
         ir_array_literal.o ir_array_slice.o                       \
         ir_array_cell.o ir_function_def.o ir_function_decl.o     \
         ir_property.o                                                        \
         iml_operation.o iml_func_frame.o iml_operand.o iml_variable.o        \
         iml_constant.o iml_register.o                                        \
         $(X86_TRGT_OBJS)

# all objects generated both from C and D source files
OBJS := $(COBJS) $(ARM_TRGT_OBJS) ui-d.o
