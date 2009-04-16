ACCENT := tools/bin/accent
PROG := xdc
CFLAGS += -Werror
CFLAGS += -Wall -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0 gobject-2.0)

OBJS := auxil.o entire.o lex.o yygrammar.o sym_table.o utils.o                \
	sem_analyze.o ui.o ast_array_constant.o ast_while.o                   \
        ast_node.o ast_compile_unit.o ast_function.o ast_data_type.o          \
        ast_basic_type.o ast_code_block.o ast_statment.o ast_expression.o     \
        ast_return.o ast_constant.o ast_int_constant.o ast_bool_constant.o    \
        ast_binary_operation.o ast_unary_operation.o ast_assigment.o          \
        ast_variable_declaration.o ast_variable_definition.o ast_if_block.o   \
        ast_variable_ref.o ast_function_call.o ast_if_else.o ast_foreach.o    \
        ast_static_array_type.o ast_array_cell_ref.o ast_array_slice_ref.o    \
        ast_auto_type.o                                                       \
        ir_symbol.o ir_variable.o ir_function.o ir_code_block.o ir_statment.o \
        ir_compile_unit.o ir_if_else.o ir_if_block.o ir_while.o ir_foreach.o  \
        java_trgt.o java_local_slot.o                                         \
        x86.o x86_reg_location.o x86_frame_offset.o


all: $(PROG)

lex.h lex.c: tokens.lex yygrammar.h
	flex -o lex.c --header-file=lex.h  tokens.lex

auxil.o: lex.h auxil.c

ui.o: ui.d
	gdc -g -c ui.d

yygrammar.c yygrammar.h: grammar.acc $(ACCENT)
	$(ACCENT) grammar.acc

tools/bin/accent:
	mkdir -p tools/bin
	cd tools/accent/accent && ./build

#
# define custom rules to compile auto-generated C files,
# as we can't compile them, as the rest of the files, with -Werror flag
#
entire.o: entire.c auxil.h lex.h
	cc -c entire.c

yygrammar.o: yygrammar.c
	cc -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) yygrammar.c

lex.o: lex.c lex.h
	cc -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) lex.c


$(PROG): $(OBJS)
	gdc -g -o $(PROG) $(LDFLAGS) $(OBJS)

# rules to run tests
unit_tests: lex.h lex.c
	make -C utests check

function_tests_java: $(PROG)
	cd tests; ./run_tests.sh --march=java

function_tests_x86: $(PROG)
	cd tests; ./run_tests.sh --march=x86

all_tests: unit_tests function_tests_java
	@echo "ALL TESTS PASSED"


clean:
	make -C tests clean
	make -C utests clean
	make -C examples clean
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h core *.class *.j *~
