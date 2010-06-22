include ../objects.mak

PROG := check_xdc

CFLAGS += -DSTATIC= -g $(shell  pkg-config --cflags glib-2.0 gobject-2.0)
LDFLAGS += $(shell  pkg-config --libs glib-2.0 gobject-2.0)

TEST_SRC := $(OBJS:%.o=../%.c)
TEST_OBJS :=  $(TEST_SRC:../%.c=%.o)
OBJS := check_label_gen.o main.o

all: $(PROG)

check: $(PROG)
	./$(PROG)

%.o: ../%.c
	gcc $(CFLAGS) -DSTATIC= -c -o $@ ../$(subst .o,.c,$@)

%.o: ../%.d
	dmd -g -c ../$(subst .o,.d,$@)


$(PROG): $(OBJS) $(TEST_OBJS)
	dmd -g -of$(PROG) -L-lgobject-2.0 -L-lglib-2.0 $(OBJS) $(TEST_OBJS) -L-lcheck

clean:
	rm -rf $(PROG) *.o *~
