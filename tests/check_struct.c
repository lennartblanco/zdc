#include <string.h>
#include <stdlib.h>

#include "check_utils.h"

struct orange
{
  int a;
  int b;
  char c;
  int d;
};

struct list
{
  struct list *next;
  void *payload;
  bool visited;
  unsigned ref_cntr;
};

unsigned
orange_sizeof();

int
orange_def(unsigned int);

int
orange_locvar(int a, int b, char c);

int
call_orangize(struct orange *arg);

unsigned
list_sizeof();

struct list *
call_list_get_next(struct list *arg);

void *
call_list_get_payload(struct list *arg);

struct list *
call_list_prepend(struct list *arg1, void *arg2);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/

void
test_orange()
{
    /* orange_sizeof() test */
    check_uint("orange_sizeof()", orange_sizeof(), 13);

    /* orange_def() tests */
    check_int("orange_def(-1)", orange_def(-1), -1);
    check_int("orange_def(0)", orange_def(0), 2);
    check_int("orange_def(1)", orange_def(1), 0);
    check_int("orange_def(2)", orange_def(2), 0xff);
    check_int("orange_def(3)", orange_def(3), 100);
    check_int("orange_def(4)", orange_def(4), -1);

    /* orange_locvar() tests */
    check_int("orange_locvar(10, 12, '+')", orange_locvar(10, 12, '+'), 10+12);
    check_int("orange_locvar(-3, 0, '+')", orange_locvar(-3, 0, '+'), -3);
    check_int("orange_locvar(3, 13, '?')", orange_locvar(3, 13, '?'), 100);

    /* orangize() tests */
    struct orange o;

    o.a = 10;
    o.b = 20;
    o.c = '+';
    check_int("orangize({10, 20, '+'})", call_orangize(&o), o.a + o.b);

    o.a = 2;
    o.b = 14;
    o.c = '-';
    check_int("orangize({2, 14, '-'})", call_orangize(&o), o.a - o.b);

    o.a = 0;
    o.b = 0;
    o.c = '?';
    check_int("orangize({0, 0, '?'})", call_orangize(&o), -1);
}

void
test_list()
{
    /* list_sizeof() test */
    check_uint("list_sizeof()", list_sizeof(), 13);

    /* list_get_next() tests */
    struct list l;

    memset(&l, 0, sizeof(l));
    check_pointer("list_get_next(l)",
                  call_list_get_next(&l),
                  NULL);

    l.next = (struct list*)0xadfefe23;
    check_pointer("list_get_next(l)",
                  call_list_get_next(&l),
                  (struct list*)0xadfefe23);

    /* list_get_payload() tests */
    l.payload = NULL;
    check_pointer("list_get_payload(l)",
                  call_list_get_payload(&l),
                  NULL);

    l.payload = (void*)0x12345678;
    check_pointer("list_get_payload(l)",
                  call_list_get_payload(&l),
                  (void*)0x12345678);

    /* list_prepend() test */
    struct list *p;
    p = call_list_prepend(&l, (void*)0xaabbccdd);
    check_cond("list_prepend(&l, 0xaabbccdd)",
               (p->next == &l) &&
               (p->payload == (void*)0xaabbccdd) &&
               (p->visited == false) &&
               (p->ref_cntr == 0));
    free(p);
}

int
main()
{
    test_orange();
    test_list();

    check_exit();
}
