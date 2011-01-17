include objects.mak
include config.default
-include config

ACCENT := tools/bin/accent
PROG := xdc
#CFLAGS += -Werror
CFLAGS += -Wall -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
DEP_DIR := .depend
CFLAGS   += -MP -MD -MF $(DEP_DIR)/$(patsubst .%,_.%,$(subst /,_,$(patsubst %.os,%.dep,$(@:.o=.dep)))) -MT $@

.PHONY: docs

all: $(PROG)

-include $(shell mkdir -p $(DEP_DIR)) $(DEP_DIR)/*.dep

lex.h: lex.c

lex.c: tokens.lex yygrammar.h
	flex -o lex.c --header-file=lex.h  tokens.lex

auxil.o: lex.h auxil.c

parser.o: lex.h parser.c

%-d.o: %.d
	dmd -debug -gc -of$@ -c $<

yygrammar.h: yygrammar.c

yygrammar.c: grammar.acc $(ACCENT)
	$(ACCENT) grammar.acc

ui-d.o: config.d

config:
	touch $@

config.d: config config.d.in
	filepp -DCONF_DEF_BACKEND=$(CONF_DEF_BACKEND) \
           -DCONF_X86_AS_CMD=$(CONF_X86_AS_CMD) \
           -DCONF_X86_LD_CMD=$(CONF_X86_LD_CMD) \
           -DCONF_ARM_AS_CMD=$(CONF_ARM_AS_CMD) \
           -DCONF_ARM_LD_CMD=$(CONF_ARM_LD_CMD) \
           config.d.in > $@

$(ACCENT):
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

$(PROG): $(OBJS)
	dmd -debug -gc -of$(PROG) $(OBJS) -L-lgobject-2.0 -L-lglib-2.0

function_tests_x86: $(PROG)
	cd tests; ./run_tests.sh --march=x86

errors_tests: $(PROG)
	cd etests; ./run_tests.sh

docs:
	make -C docs

clean:
	make -C docs clean
	make -C tests clean
	make -C etests clean
	make -C examples clean
	rm -rf $(DEP_DIR)
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h core *.class *.j *~ config.d
