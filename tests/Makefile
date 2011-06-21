XDC=../xdc

check_empty: empty.o check_utils.o check_empty.c
	$(CC) $(CFLAGS) -o $@ $^

check_rets: rets.o check_utils.o check_rets.c
	$(CC) $(CFLAGS) -o $@ $^

check_only_comments: only_comments.o check_utils.o check_only_comments.c
	$(CC) $(CFLAGS) -o $@ $^

check_comments: comments.o check_utils.o check_comments.c
	$(CC) $(CFLAGS) -o $@ $^

check_neg: neg.o check_utils.o check_neg.c
	$(CC) $(CFLAGS) -o $@ $^

check_func_call: func_call.o check_utils.o check_func_call.c
	$(CC) $(CFLAGS) -o $@ $^

check_implicit_cast: implicit_cast.o check_utils.o check_implicit_cast.c
	$(CC) $(CFLAGS) -o $@ $^

check_bool_op: bool_op.o check_utils.o check_bool_op.c
	$(CC) $(CFLAGS) -o $@ $^

check_uint_op: uint_op.o check_utils.o check_uint_op.c
	$(CC) $(CFLAGS) -o $@ $^

check_nested_blocks: nested_blocks.o check_utils.o check_nested_blocks.c
	$(CC) $(CFLAGS) -o $@ $^

check_if_else: if_else.o check_utils.o check_if_else.c
	$(CC) $(CFLAGS) -o $@ $^

check_fact: fact.o check_utils.o check_fact.c
	$(CC) $(CFLAGS) -o $@ $^

check_extern_c: extern_c.o check_utils.o check_extern_c.c
	$(CC) $(CFLAGS) -o $@ $^

check_while_loop: while_loop.o check_utils.o check_while_loop.c
	$(CC) $(CFLAGS) -o $@ $^

check_stat_array: stat_array.o check_utils.o check_stat_array.c
	$(CC) $(CFLAGS) -o $@ $^

check_dyn_array: dyn_array.o check_utils.o check_dyn_array.c
	$(CC) $(CFLAGS) -o $@ $^

check_foreach: foreach.o check_utils.o check_foreach.c
	$(CC) $(CFLAGS) -o $@ $^

check_module_test: module_test.o check_utils.o check_module_test.c
	$(CC) $(CFLAGS) -o $@ $^

check_pkg_name_tst: pkg_name_tst.o check_utils.o check_pkg_name_tst.c
	$(CC) $(CFLAGS) -o $@ $^

check_imports: imports.o fact.o rets.o neg.o check_utils.o check_imports.c
	$(CC) $(CFLAGS) -o $@ $^

check_vars: vars.o check_utils.o check_vars.c
	$(CC) $(CFLAGS) -o $@ $^

check_char_literals: char_literals.o check_utils.o check_char_literals.c $(ARCH_SPECIFIC)
	$(CC) $(CFLAGS) -o $@ $^

check_properties: properties.o check_utils.o check_properties.c
	$(CC) $(CFLAGS) -o $@ $^

check_immutable: immutable.o check_utils.o check_immutable.c $(ARCH_SPECIFIC)
	$(CC) $(CFLAGS) -o $@ $^

check_enums: enums.o check_utils.o check_enums.c
	$(CC) $(CFLAGS) -o $@ $^

check_struct: struct.o check_utils.o check_struct.c
	$(CC) $(CFLAGS) -o $@ $^

check_alias: alias.o check_utils.o check_alias.c $(ARCH_SPECIFIC)
	$(CC) $(CFLAGS) -o $@ $^

check_basic_types: basic_types.o check_utils.o check_basic_types.c
	$(CC) $(CFLAGS) -o $@ $^

check_pointers: pointers.o check_utils.o check_pointers.c
	$(CC) -g $(CFLAGS) -o $@ $^

check_arithm: arithm.o check_utils.o check_arithm.c
	$(CC) $(CFLAGS) -o $@ $^

check_incdec_ops: incdec_ops.o check_utils.o check_incdec_ops.c $(ARCH_SPECIFIC)
	$(CC) $(CFLAGS) -o $@ $^

check_hex: hex.o check_utils.o check_hex.c
	$(CC) $(CFLAGS) -o $@ $^

check_eels: check_eels.sh eels
	cp $< $@

check_str_literals: check_str_literals.sh str_literals
	cp $< $@

eels: eels.d $(XDC)
	$(XDC) -o $@ $(DFLAGS) $<

str_literals: str_literals.d $(XDC)  $(ARCH_SPECIFIC)
	$(XDC) -o $@ $(DFLAGS) $<

%.o: %.d $(XDC)
	$(XDC) -o $@ $(DFLAGS) -c $<

check_utils.o: check_utils.c
	$(CC) $(CFLAGS) -c $?

dummy_gc.o: dummy_gc.c
	$(CC) $(CFLAGS) -c $?

clean:
	rm -f *.s *.o *~ core check_empty check_rets \
check_only_comments check_comments check_neg check_func_call check_imports \
check_bool_op check_extern_c check_fact check_foreach check_if_else \
check_implicit_cast check_module_test check_nested_blocks check_pkg_name_tst \
check_stat_array check_dyn_array check_uint_op check_while_loop eels \
check_properties check_uninit_vars check_char_literals check_immutable \
str_literals check_enums check_pointers check_arithm check_alias check_struct \
check_incdec_ops check_hex check_basic_types
	rm -rf build
