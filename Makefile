include objects.mak

ACCENT := tools/bin/accent
PROG := xdc
#CFLAGS += -Werror
CFLAGS += -Wall -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0 gobject-2.0)

.PHONY: docs

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
	gcc -g -c entire.c

yygrammar.o: yygrammar.c
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) yygrammar.c

lex.o: lex.c lex.h
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) lex.c

test: $(OBJS) test.o
	gcc -g -o test $(LDFLAGS) $(OBJS) test.o

$(PROG): $(OBJS) ui.o
	gdc -g -o $(PROG) $(LDFLAGS) $(OBJS) ui.o

# rules to run tests
unit_tests: lex.h lex.c
	make -C utests check

function_tests_java: $(PROG)
	cd tests; ./run_tests.sh --march=java

function_tests_x86: $(PROG)
	cd tests; ./run_tests.sh --march=x86

all_tests: unit_tests function_tests_java
	@echo "ALL TESTS PASSED"


docs:
	make -C docs

clean:
	make -C docs clean
	make -C tests clean
	make -C utests clean
	make -C examples clean
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h core *.class *.j *~
