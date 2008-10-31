ACCENT := tools/bin/accent
PROG := xdc
CFLAGS += -g $(shell  pkg-config --cflags glib-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0)

OBJS := auxil.o entire.o lex.o yygrammar.o ast.o sym_table.o \
	ast_dbg_dump.o gen_java.o sem_analyze.o ir.o ui.o


all: $(PROG)

lex.h lex.c: tokens.lex yygrammar.h
	flex -o lex.c --header-file=lex.h  tokens.lex

ui.o: ui.d
	gdc -c ui.d

yygrammar.c yygrammar.h: grammar.acc $(ACCENT)
	$(ACCENT) grammar.acc

tools/bin/accent:
	mkdir -p tools/bin
	cd tools/accent/accent && ./build

$(PROG): $(OBJS)
	gdc -o $(PROG) $(LDFLAGS) $(OBJS)

clean:
	rm -rf $(PROG) *.o lex.c lex.h yygrammar.c yygrammar.h
