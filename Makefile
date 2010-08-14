include objects.mak

ACCENT := tools/bin/accent
PROG := xdc
#CFLAGS += -Werror
CFLAGS += -Wall -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
DEPS = $(COBJS:.o=.dep)

.PHONY: docs

all: $(PROG)

lex.h lex.c: tokens.lex yygrammar.h
	flex -o lex.c --header-file=lex.h  tokens.lex

auxil.o: lex.h auxil.c

%.o: %.d
	dmd -g -c $?

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
	gcc -g -c entire.c

yygrammar.o: yygrammar.c
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) yygrammar.c

lex.o: lex.c lex.h
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) lex.c

%.dep : %.c
	gcc $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

$(PROG): $(OBJS)
	dmd -g -of$(PROG) $(OBJS) -L-lgobject-2.0 -L-lglib-2.0

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif

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
	make -C utests clean
	make -C examples clean
	rm -f *.dep
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h core *.class *.j *~
