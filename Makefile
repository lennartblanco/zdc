include objects.mak

ACCENT := tools/bin/accent
PROG := zdc
GEN_CONFIG :=./gen_conf.rb
CONFIGS := $(shell echo config; [ -e config.local ] && echo config.local)
TARGETS := $(patsubst $(PROG),,$(shell $(GEN_CONFIG) --targets $(CONFIGS)))
CFLAGS += -Wall -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
BLD_DIR := build
DEP_DIR := $(BLD_DIR)
CFLAGS   += -MP -MD -MF $(DEP_DIR)/$(patsubst .%,_.%,$(subst /,_,$(patsubst %.os,%.dep,$(@:.o=.dep)))) -MT $@

VPATH = $(BLD_DIR)

all: $(TARGETS)

-include $(shell mkdir -p $(DEP_DIR)) $(DEP_DIR)/*.dep

$(TARGETS): $(PROG)
	ln -f -s $< $@

lex.h: lex.c

lex.c: tokens.lex yygrammar.h
	flex -o lex.c --header-file=lex.h  tokens.lex

config.local:
	# nop

config.mixin: $(CONFIGS)
	$(GEN_CONFIG) $(CONFIGS) > $@

auxil.o: lex.h auxil.c

parser.o: lex.h parser.c

config-d.o: config.mixin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(BLD_DIR)/$@

%-d.o: %.d
	dmd -debug -gc -J. -of$(BLD_DIR)/$@ -c $<

yygrammar.h: yygrammar.c

yygrammar.c: grammar.acc $(ACCENT)
	$(ACCENT) grammar.acc

$(ACCENT):
	mkdir -p tools/bin
	cd tools/accent/accent && ./build

#
# define custom rules to compile auto-generated C files,
# as we can't compile them, as the rest of the files, with -Werror flag
#
entire.o: entire.c auxil.h lex.h
	gcc -g -c entire.c -o $(BLD_DIR)/$@

yygrammar.o: yygrammar.c
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) yygrammar.c -o $(BLD_DIR)/$@

lex.o: lex.c lex.h
	gcc -g -c $(shell  pkg-config --cflags glib-2.0 gobject-2.0) lex.c -o $(BLD_DIR)/$@

$(PROG): $(OBJS)
	cd $(BLD_DIR); dmd -debug -gc -of../$(PROG) $(OBJS) -L-lgobject-2.0 -L-lglib-2.0

function_tests: $(PROG)
	cd tests; ./run_tests.rb

errors_tests: $(PROG)
	cd etests; ./run_tests.sh

docs:
	make -C docs

clean:
	make -C tests clean
	make -C etests clean
	rm -rf $(BLD_DIR)
	rm -rf $(PROG) $(TARGETS) *.d.deps lex.c lex.h yygrammar.c yygrammar.h core *~ config.mixin
