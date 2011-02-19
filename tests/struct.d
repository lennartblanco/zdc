struct orange
{
  int a;
  int b;
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

extern (C) uint
list_sizeof()
{
    return list.sizeof;
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
