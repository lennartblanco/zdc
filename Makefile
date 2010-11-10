include objects.mak

ACCENT := tools/bin/accent
PROG := xdc
#CFLAGS += -Werror
CFLAGS += -Wall -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
DEPS = $(COBJS:.o=.dep)
DEP_DIR := .depend
CFLAGS   += -MP -MD -MF $(DEP_DIR)/$(patsubst .%,_.%,$(subst /,_,$(patsubst %.os,%.dep,$(@:.o=.dep)))) -MT $@

.PHONY: docs

all: $(DEP_DIR) $(PROG)

-include $(DEP_DIR)/*.dep

lex.h lex.c: tokens.lex yygrammar.h
	flex -o lex.c --header-file=lex.h  tokens.lex

auxil.o: lex.h auxil.c

%.o: %.d
	dmd -debug -g -c $?

yygrammar.c yygrammar.h: grammar.acc $(ACCENT)
	$(ACCENT) grammar.acc

$(ACCENT):
	mkdir -p tools/bin
	cd tools/accent/accent && ./build

$(DEP_DIR):
	mkdir -p $(DEP_DIR)
#
# define custom rules to compile auto-generated C files,
# as we can't compile them, as the rest of the files, with -Werror flag
#
entire.o: entire.c auxil.h lex.h
	gcc -g -c entire.c

yygrammar.o: yygrammar.c
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) yygrammar.c

lex.o: lex.c lex.h
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) lex.c

$(PROG): $(OBJS)
	dmd -debug -g -of$(PROG) $(OBJS) -L-lgobject-2.0 -L-lglib-2.0

# rules to run tests
unit_tests: lex.h lex.c
	make -C utests check

function_tests_java: $(PROG)
	cd tests; ./run_tests.sh --march=java

function_tests_x86: $(PROG)
	cd tests; ./run_tests.sh --march=x86

errors_tests: $(PROG)
	cd etests; ./run_tests.sh

all_tests: unit_tests function_tests_java errors_tests
	@echo "ALL TESTS PASSED"


docs:
	make -C docs

clean:
	make -C docs clean
	make -C tests clean
	make -C etests clean
	make -C examples clean
	rm -rf $(DEP_DIR)
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h core *.class *.j *~
