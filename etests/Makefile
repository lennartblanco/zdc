all:
	@./run_tests.sh

syntx_err: syntx_err.d
array_slice: array_slice.d
assigment: assigment.d
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
module_syntx_err1: module_syntx_err1.d
module_syntx_err2: module_syntx_err2.d
imprt_not_found: imprt_not_found.d
imprt_syntx_err: imprt_syntx_err.d syntx_err.d
ambig_imprt: ambig_imprt.d imp_mod_a.d imp_mod_b.d
unknown_prop: unknown_prop.d

%.d: %.inv_d
	cp $? $@

clean:
	rm -f errors_log *.d
