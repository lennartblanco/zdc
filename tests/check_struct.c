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

int
orange_address_of(int b, char c, bool via_ptr);

unsigned
list_sizeof();

struct list *
call_list_get_next(struct list *arg);

void *
call_list_get_payload(struct list *arg);

struct list *
call_list_prepend(struct list *arg1, void *arg2);

int
all_types_get_def_val(unsigned testno);

bool
test_all_types_set_val();

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/

void
test_orange()
{
    /* orange_sizeof() test */
    check_uint("orange_sizeof()", orange_sizeof(), 20);

    /* orange_def() tests */
    check_int("orange_def(-1)", orange_def(-1), -1);
    check_int("orange_def(100)", orange_def(100), -1);
    check_int("orange_def(0)", orange_def(0), 2);
    check_int("orange_def(1)", orange_def(1), 0);
    check_int("orange_def(2)", orange_def(2), 0xff);
    check_int("orange_def(3)", orange_def(3), 100);
    check_int("orange_def(4)", orange_def(4), 10);

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

    /* orange_address_of() tests */
    check_int("orange_address_of(10, '+', false)",
              orange_address_of(10, '+', false), 2 + 10);
    check_int("orange_address_of(-2, '-', false)",
              orange_address_of(-2, '-', false), 2 - (-2));
    check_int("orange_address_of(-10, '+', true)",
              orange_address_of(-10, '+', true), 2 + (-10));
    check_int("orange_address_of(200, '?', true)",
              orange_address_of(200, '?', true), -1);

}

void
test_list()
{
    /* list_sizeof() test */
    check_uint("list_sizeof()", list_sizeof(), 16);

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

void
test_all_types()
{
    /* all_types_get_def_val() tests */
    check_int("all_types_get_def_val(100)", all_types_get_def_val(100), -1);
    check_int("all_types_get_def_val(0)", all_types_get_def_val(0), 'a');
    check_int("all_types_get_def_val(1)", all_types_get_def_val(1), true);
    check_int("all_types_get_def_val(2)", all_types_get_def_val(2), -2);
    check_int("all_types_get_def_val(3)", all_types_get_def_val(3), 202);
    check_int("all_types_get_def_val(4)", all_types_get_def_val(4), -5);
    check_int("all_types_get_def_val(5)", all_types_get_def_val(5), 30000);
    check_int("all_types_get_def_val(6)", all_types_get_def_val(6), -3);
    check_int("all_types_get_def_val(7)", all_types_get_def_val(7), 0);

    /* test_all_types_set_val() tests */
    check_bool("test_all_types_set_val()", test_all_types_set_val(), true);
}

int
main()
{
    test_orange();
    test_list();
    test_all_types();

    check_exit();
}
