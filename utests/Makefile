PROG := check_xdc

CFLAGS += -g $(shell  pkg-config --cflags glib-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0)

TEST_SRC := $(wildcard ../*.c)
TEST_OBJS :=  $(TEST_SRC:../%.c=%.o)
OBJS := check_java_trgt.o  main.o

all: $(PROG)

check: $(PROG)
	./$(PROG)

$(TEST_OBJS): $(TEST_SRC)
	gcc $(CFLAGS) -DSTATIC= -c -o $@ ../$(subst .o,.c,$@)


$(PROG): $(OBJS) $(TEST_OBJS)
	gcc -o $(PROG) $(LDFLAGS) $(OBJS) $(TEST_OBJS) -lcheck

clean:
	rm -rf $(PROG) *.o
