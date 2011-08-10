all:
	@./run_tests.sh

syntx_err: syntx_err.d
array_slice: array_slice.d
assignment: assignment.d
unknown_symb: unknown_symb.d
sym_clash: sym_clash.d
func_call: func_call.d
types: types.d
array_element: array_element.d
init_exp: init_exp.d
arry_literal: arry_literal.d
entry_point: entry_point.d
bin_ops: bin_ops.d
exprs: exprs.d
foreach: foreach.d
foreach_range: foreach_range.d
foreach_range_symbs: foreach_range_symbs.d
module_syntx_err1: module_syntx_err1.d
module_syntx_err2: module_syntx_err2.d
imprt_not_found: imprt_not_found.d
imprt_syntx_err: imprt_syntx_err.d syntx_err.d
ambig_imprt: ambig_imprt.d imp_mod_a.d imp_mod_b.d
unknown_prop: unknown_prop.d
invalid_prop: invalid_prop.d
invalid_returns: invalid_returns.d
immutable: immutable.d
esc_seq: esc_seq.d
alias: alias.d
usr_types_conflict: usr_types_conflict.d
unknow_usr_type: unknow_usr_type.d
enum_init_err: enum_init_err.d
structs: structs.d
inc_dec_ops: inc_dec_ops.d
inv_break: inv_break.d
for_loop: for_loop.d

%.d: %.inv_d
	cp $? $@

clean:
	rm -f errors_log *.d
