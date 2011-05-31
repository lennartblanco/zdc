struct orange
{
  int a, b;
  char c;
}

alias void* ptr;
alias bool flag;

struct list
{
  list *next;
  ptr payload;
  flag visited;
  uint ref_cntr;
}

extern (C) list *malloc(uint);

extern (C) uint
orange_sizeof()
{
    return orange.sizeof;
}

int
orangize(orange *orng)
{
  if (orng.c == '+')
  {
    return orng.a + orng.b;
  }
  else if (orng.c == '-')
  {
    return orng.a - orng.b;
  }

  return -1;
}

extern (C) uint
list_sizeof()
{
    return list.sizeof;
}

list *
list_get_next(list *l)
{
   return l.next;
}

ptr
list_get_payload(list *l)
{
  return l.payload;
}

list *
list_prepend(list *l, ptr payload)
{
  list *nl;

  nl = malloc(list.sizeof);
  nl.next = l;
  nl.payload = payload;
  nl.visited = false;
  nl.ref_cntr = 0;

  return nl;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
    int
    call_orangize(orange *arg)
    {
        return orangize(arg);
    }

    list *
    call_list_get_next(list *arg)
    {
        return list_get_next(arg);
    }

    ptr
    call_list_get_payload(list *arg)
    {
        return list_get_payload(arg);
    }

    list *
    call_list_prepend(list *arg1, ptr arg2)
    {
        return list_prepend(arg1, arg2);
    }
}
