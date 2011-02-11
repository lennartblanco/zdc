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
