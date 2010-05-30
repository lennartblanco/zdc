XDC=../xdc

check_empty: empty.o check_utils.o check_empty.c
	gcc $(CFLAGS) -o $@ $^

check_rets: rets.o check_utils.o check_rets.c
	gcc $(CFLAGS) -o $@ $^

check_only_comments: only_comments.o check_utils.o check_only_comments.c
	gcc $(CFLAGS) -o $@ $^

check_comments: comments.o check_utils.o check_comments.c
	gcc $(CFLAGS) -o $@ $^

check_neg: neg.o check_utils.o check_neg.c
	gcc $(CFLAGS) -o $@ $^

check_func_call: func_call.o check_utils.o check_func_call.c
	gcc $(CFLAGS) -o $@ $^

check_implicit_cast: implicit_cast.o check_utils.o check_implicit_cast.c
	gcc $(CFLAGS) -o $@ $^

check_bool_op: bool_op.o check_utils.o check_bool_op.c
	gcc $(CFLAGS) -o $@ $^

check_uint_op: uint_op.o check_utils.o check_uint_op.c
	gcc $(CFLAGS) -o $@ $^

check_nested_blocks: nested_blocks.o check_utils.o check_nested_blocks.c
	gcc $(CFLAGS) -o $@ $^

check_if_else: if_else.o check_utils.o check_if_else.c
	gcc $(CFLAGS) -o $@ $^

check_fact: fact.o check_utils.o check_fact.c
	gcc $(CFLAGS) -o $@ $^

check_extern_c: extern_c.o check_utils.o check_extern_c.c
	gcc $(CFLAGS) -o $@ $^

check_while_loop: while_loop.o check_utils.o check_while_loop.c
	gcc $(CFLAGS) -o $@ $^

check_stat_array: stat_array.o check_utils.o check_stat_array.c
	gcc $(CFLAGS) -o $@ $^

check_dyn_array: dyn_array.o check_utils.o check_dyn_array.c
	gcc $(CFLAGS) -o $@ $^

check_foreach: foreach.o check_utils.o check_foreach.c
	gcc $(CFLAGS) -o $@ $^

check_module_test: module_test.o check_utils.o check_module_test.c
	gcc $(CFLAGS) -o $@ $^

check_pkg_name_tst: pkg_name_tst.o check_utils.o check_pkg_name_tst.c
	gcc $(CFLAGS) -o $@ $^

check_imports: imports.o fact.o rets.o neg.o check_utils.o check_imports.c
	gcc $(CFLAGS) -o $@ $^

check_uninit_vars: uninit_vars.o check_utils.o check_uninit_vars.c
	gcc $(CFLAGS) -o $@ $^

check_char_literals: char_literals.o check_utils.o check_char_literals.c
	gcc $(CFLAGS) -o $@ $^

check_properties: properties.o check_utils.o check_properties.c
	gcc $(CFLAGS) -o $@ $^

check_immutable: immutable.o check_utils.o check_immutable.c
	gcc $(CFLAGS) -o $@ $^

check_enums: enums.o check_utils.o check_enums.c
	gcc $(CFLAGS) -o $@ $^

check_eels: eels

check_str_literals: str_literals

eels: eels.d
	$(XDC) $(DFLAGS) $?

str_literals: str_literals.d
	$(XDC) $(DFLAGS) $?

%.o: %.d
	$(XDC) $(DFLAGS) -c $?

check_utils.o: check_utils.c
	gcc $(CFLAGS) -c $?

clean:
	rm -f *.s *.class *.o *.j *~ core check_empty check_rets \
check_only_comments check_comments check_neg check_func_call check_imports \
check_bool_op check_extern_c check_fact check_foreach check_if_else \
check_implicit_cast check_module_test check_nested_blocks check_pkg_name_tst \
check_stat_array check_dyn_array check_uint_op check_while_loop eels \
check_properties check_uninit_vars check_char_literals check_immutable \
str_literals check_enums
