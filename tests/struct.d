enum speed { slow = 10, fast, swoosh }

struct orange
{
  int a = 2, b;
  char c;
  uint d = 100;
  speed e;
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

extern (C) int
orange_def(uint n)
{
  /* test default initialization of a struct */
  orange o;

  if (n == 0) { return o.a; }
  if (n == 1) { return o.b; }
  if (n == 2) { return o.c; }
  if (n == 3) { return o.d; }
  if (n == 4) { return o.e; }

  return -1;
}

extern (C) int
orange_locvar(int a, int b, char c)
{
  orange o;

  o.a = a;
  o.b = b;
  o.c = c;

  if (o.c == '+')
  {
    return o.a + o.b;
  }

  return o.d;
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

extern (C) int
orange_address_of(int b, char c, bool via_ptr)
{
  orange o;
  orange *ptr = &o;

  o.b = b;
  ptr.c = c;

  if (via_ptr)
  {
    return orangize(ptr);
  }
  return orangize(&o);
}

/* test reference struct function parameter */
void
orange_ref(ref orange orng)
{
  orng.a = orng.b - 10;
}

extern (C) bool
orange_ref_test()
{
  orange o1;
  o1.b = 11;
  orange_ref(o1);

  orange o2;
  o2.b = 98;
  orange_ref(o2);

  return o1.a == 11 - 10 && o2.a == 98 - 10;
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

  nl = new list;
  nl.next = l;
  nl.payload = payload;
  nl.visited = false;
  nl.ref_cntr = 0;

  return nl;
}

/* struct containing files of all basic data types */
struct all_types
{
    char char_field = 'a';
    bool bool_field = true;
    byte byte_field = -2;
    ubyte ubyte_field = 202;
    short short_field = -5;
    ushort ushort_field = 30000;
    int int_field = -3;
    uint uint_field;

    int get_int_field()
    {
        return this.int_field;
    }

    void set_int_field(int v)
    {
        this.int_field = v;
    }
}

/* test reading default value of each struct field */
extern (C) int
all_types_get_def_val(uint testno)
{
  all_types at;

  if (testno == 0)
  {
    return at.char_field;
  }
  else if (testno == 1)
  {
    return at.bool_field;
  }
  else if (testno == 2)
  {
    return at.byte_field;
  }
  else if (testno == 3)
  {
    return at.ubyte_field;
  }
  else if (testno == 4)
  {
    return at.short_field;
  }
  else if (testno == 5)
  {
    return at.ushort_field;
  }
  else if (testno == 6)
  {
    return at.int_field;
  }
  else if (testno == 7)
  {
    return at.uint_field;
  }

  return -1;
}

/* test writing to each field of the struct */
void
all_types_set_val(all_types *at, uint field_no, int val)
{
  if (field_no == 0)
  {
    at.char_field = cast(char)val;
  }
  else if (field_no == 1)
  {
    at.bool_field = cast(bool)val;
  }
  else if (field_no == 2)
  {
    at.byte_field = cast(byte)val;
  }
  else if (field_no == 3)
  {
    at.ubyte_field = cast(byte)val;
  }
  else if (field_no == 4)
  {
    at.short_field = cast(byte)val;
  }
  else if (field_no == 5)
  {
    at.ushort_field = cast(ushort)val;
  }
  else if (field_no == 6)
  {
    at.int_field = val;
  }
  else if (field_no == 7)
  {
    at.uint_field = val;
  }
}

/*
 * write values to structs field and check the correct value was written
 *
 * @return true if test was successfull, false on failure
 */
extern (C) bool
test_all_types_set_val()
{
  all_types at;

  all_types_set_val(&at, 0, 'x');   // char_field
  all_types_set_val(&at, 1, false); // bool_field
  all_types_set_val(&at, 2, -100);  // byte_field
  all_types_set_val(&at, 3, 247);   // ubyte_field
  all_types_set_val(&at, 4, -5);    // short_field
  all_types_set_val(&at, 5, 42);    // ushort_field
  all_types_set_val(&at, 6, 0);     // int_field
  all_types_set_val(&at, 7, 312);   // uint_field

  return at.char_field == 'x' &&
         at.bool_field == false &&
         at.byte_field == -100 &&
         at.ubyte_field == 247 &&
         at.short_field == -5 &&
         at.ushort_field == 42 &&
         at.int_field == 0 &&
         at.uint_field == 312;
}

extern (C) bool
test_all_types_methods()
{
  all_types at;
  all_types *ptr = &at;
  int r1;
  int r2;
  int r3;

  at.set_int_field(100);
  r1 = at.get_int_field();
  r2 = ptr.get_int_field();

  ptr.set_int_field(200);
  r3 = ptr.get_int_field();


  return r1 == 100 && r2 == 100 && r3 == 200;
}

struct opaque;

extern (C) opaque *
opaque_new(int a, int b);

extern (C) int
opaque_sum(opaque *self);

extern (C) int
opaque_ptr(int a, int b)
{
    opaque *o = opaque_new(a, b);

    return opaque_sum(o);
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
