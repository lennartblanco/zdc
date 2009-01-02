ACCENT := tools/bin/accent
PROG := xdc
CFLAGS += -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0 gobject-2.0)

OBJS := auxil.o entire.o lex.o yygrammar.o ast.o sym_table.o                  \
	ast_dbg_dump.o java_trgt.o sem_analyze.o ir.o ui.o                    \
        ast_node.o ast_compile_unit.o ast_function.o ast_data_type.o          \
        ast_basic_type.o ast_code_block.o ast_statment.o ast_expression.o     \
        ast_return.o ast_constant.o ast_int_constant.o ast_bool_constant.o    \
        ast_binary_operation.o ast_unary_operation.o ast_assigment.o          \
        ast_variable_declaration.o ast_variable_definition.o ast_if_block.o   \
        ast_variable_ref.o ast_scalar_variable_ref.o ast_function_call.o      \
        ast_if_else.o ast_static_array_type.o ast_array_cell_ref.o


all: $(PROG)

all_tests: $(PROG)
	cd tests; ./run_tests.sh
	make -C utests check
	@echo "ALL TESTS PASSED"

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

$(PROG): $(OBJS)
	gdc -g -o $(PROG) $(LDFLAGS) $(OBJS)

clean:
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h *~
