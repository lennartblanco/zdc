X86_TRGT_COBJS := x86.o
X86_TRGT_DOBJS := x86-d.o

ARM_TRGT_OBJS := arm-d.o arm_util-d.o

# objects generated from C source files
COBJS := auxil.o entire.o parser.o lex.o yygrammar.o sym_table.o utils.o      \
         ut_range.o errors.o types.o const_fold.o                             \
         ast_to_ir.o sem_analyze.o sem_analyze_validate.o                     \
         ast_node.o ast_module.o ast_array_literal.o ast_code_block.o         \
         ast_statment.o ast_function_def.o ast_function_decl.o                \
         ast_expression.o ast_return.o ast_constant.o ast_int_constant.o      \
         ast_uint_constant.o ast_char_constant.o ast_bool_constant.o          \
         ast_import.o ast_binary_operation.o ast_unary_operation.o            \
         ast_postfix_exp.o ast_assignment.o ast_variable_declaration.o        \
         ast_variable_definition.o ast_if_block.o ast_ident.o ast_extern.o    \
         ast_protection.o ast_function_call.o ast_if_else.o ast_while.o       \
         ast_for.o ast_foreach.o ast_foreach_range.o ast_break.o ast_null.o   \
         ast_array_cell_ref.o ast_array_slice_ref.o ast_string_literal.o      \
         ast_user_type.o ast_alias.o ast_enum.o ast_enum_member.o             \
         ast_struct.o ast_ptr_dref.o ast_address_of.o ast_cast.o              \
         ast_conditional.o ast_declaration.o ast_attributes.o ast_attribute.o \
         ast_declaration_block.o                                              \
         dt_data_type.o dt_auto.o dt_void.o dt_basic.o dt_pointer.o dt_user.o \
         dt_alias.o dt_enum.o dt_struct.o dt_array.o dt_static_array.o        \
         dt_name.o                                                            \
         ir_node.o ir_symbol.o ir_variable.o ir_function.o ir_literal.o       \
         ir_enum_member.o ir_code_block.o ir_statment.o ir_cast.o             \
         ir_constant.o ir_null.o ir_basic_constant.o ir_binary_operation.o    \
         ir_conditional.o ir_unary_operation.o ir_module.o ir_if_else.o       \
         ir_if_block.o ir_loop.o ir_while.o ir_for.o ir_foreach.o             \
         ir_foreach_range.o ir_break.o ir_expression.o ir_return.o            \
         ir_function_call.o ir_assignment.o ir_array_literal.o                \
         ir_array_slice.o ir_array_cell.o ir_ptr_dref.o ir_address_of.o       \
         ir_function_def.o ir_function_decl.o ir_property.o ir_struct.o       \
         ir_struct_member.o ir_struct_literal.o ir_dot.o ir_ident.o           \
         ir_var_value.o ir_var_ref.o ir_call.o ir_method_call.o ir_scope.o    \
         iml_operation.o iml_function.o iml_operand.o iml_variable.o          \
         iml_constant.o                                                       \
         ir_to_iml.o                                                          \
         GType.o


# all objects generated both from C and D source files
OBJS := $(COBJS) $(X86_TRGT_COBJS) $(X86_TRGT_DOBJS) $(ARM_TRGT_OBJS)         \
         GList-d.o GSList-d.o ir-d.o iml-d.o ui-d.o auxil-d.o sym_table-d.o   \
         data_section-d.o config-d.o gt_dispatcher-d.o ir_printer-d.o        \
         iml_printer-d.o
