extern (C) void *malloc(uint);

int *
def_int_ptr()
{
    int *ptr;

    return ptr;
}

char *
def_char_ptr()
{
    char *ptr;
    
    return ptr;
}

// will allocate 8 bytes, which must be free():ed
void *
malloced_ptr()
{
    return malloc(8);
}

void
divide(int divident, int divisor,
       int *quotient, int *reminder)
{
  *quotient = divident / divisor;
  *reminder = divident % divisor;
}

/* test &-operator on basic data type variables */
extern (C) int
addr_of_int(int a, int b, bool ret_divident)
{
  int q;
  int r;
  int *res_ptr;

  divide(a, b, &q, &r);

  if (ret_divident)
  {
    res_ptr = &q;
  }
  else
  {
    res_ptr = &r;
  }

  return *res_ptr;
}

void
compare(int left, int right,
        bool *equal,
        bool *left_less_then_right)
{
  if (left == right) // test assigning writing bool constant via pointer
  {
    *equal = true;
  }
  else
  {
    *equal = false;
  }

  // test saving expression result via a pointer
  *left_less_then_right = left < right;
}

// test using uint pointer dereference operation in expressions
uint
uint_ptr_dref(uint *ptr, bool simple)
{
  if (simple)
  {
    return *ptr;
  }

  uint tmp = *ptr;
  uint a;

  a = *ptr / 2;

  return *ptr + a + tmp;
}

// test using char pointer dereference operation in expressions
bool
char_ptr_dref(char *c)
{
  if ('A' <= *c && *c <= 'Z')
  {
     return true;
  }

  return false;
}

// test using pointer to aliased type as function argument

alias int myint;

int
alias_ptr_arg(myint *arg)
{
  int res;

  res = *arg - 1;
  *arg = res + 2;

  return res;
}


// test pointer and integer addition
extern (C) int *
ipointer_int_addition(int *ptr, int i, bool pointer_first)
{
  int *res;

  if (pointer_first)
  {
    res = ptr + i;
  }
  else
  {
    res = i + ptr;
  }

  return res;
}

// test char pointer and integer addition
extern (C) char *
cpointer_int_addition(char *ptr, uint i)
{
  return ptr + i;
}

// test bool pointer and character addition
extern (C) bool *
bpointer_char_addition(char c, bool *ptr)
{
  bool *r;

  r = c + ptr;

  return r;
}

// test substracting an integer from a pointer
extern (C) uint *
pointer_substraction(uint *ptr, uint i, bool use_arg)
{
  if (use_arg)
  {
    return ptr - i;
  }

  return ptr - 3;
}

// test substracting a pointer from pointer
extern (C) int
pointer_pointer_substraction(int *l, int *r)
{
  return l - r;
}

// test dereferencing pointer arithmetic expression
extern (C) int
ptr_offset_access(int *ptr, uint offset)
{
  return *(ptr + offset);
}

// test pointer comparison expressions
extern (C) bool
ptr_comp(char *left, char *right, int op_type)
{
  if (op_type == 0)
  {
    return left == right;
  }
  else if (op_type == 1)
  {
    return left != right;
  }
  else if (op_type == 2)
  {
    return left < right;
  }
  else if (op_type == 3)
  {
    return left <= right;
  }
  else if (op_type == 4)
  {
    return left > right;
  }
  else if (op_type == 5)
  {
    return left >= right;
  }

  return false;
}

// test void* and int* comparisons
extern (C) bool
void_ptr_comp(void *left, int *right, int op_type)
{
  if (op_type == 0)
  {
    return left == right;
  }
  else if (op_type == 1)
  {
    return left != right;
  }
  else if (op_type == 2)
  {
    return left < right;
  }
  else if (op_type == 3)
  {
    return left <= right;
  }
  else if (op_type == 4)
  {
    return left > right;
  }
  else if (op_type == 5)
  {
    return left >= right;
  }

  return false;
}

// test comparing uint pointer with null expression
extern (C) bool
null_ptr_comp(uint* ptr, int op_type)
{
  if (op_type == 0)
  {
    return null == ptr;
  }
  else if (op_type == 1)
  {
    return null != ptr;
  }
  else if (op_type == 2)
  {
    return null < ptr;
  }
  else if (op_type == 3)
  {
    return ptr <= null;
  }
  else if (op_type == 4)
  {
    return ptr > null;
  }
  else if (op_type == 5)
  {
    return ptr >= null;
  }

  return false;
}


/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int *
  call_def_int_ptr()
  {
    return def_int_ptr();
  }

  char *
  call_def_char_ptr()
  {
    return def_char_ptr();
  }

  void *
  call_malloced_ptr()
  {
    return malloced_ptr();
  }

  void
  call_compare(int arg1, int arg2, bool *arg3, bool *arg4)
  {
    compare(arg1, arg2, arg3, arg4);
  }

  void
  call_divide(int arg1, int arg2, int *arg3, int *arg4)
  {
    divide(arg1, arg2, arg3, arg4);
  }

  uint
  call_uint_ptr_dref(uint *arg1, bool arg2)
  {
    return uint_ptr_dref(arg1, arg2);
  }

  bool
  call_char_ptr_dref(char *arg)
  {
    return char_ptr_dref(arg);
  }

  int
  call_alias_ptr_arg(myint *arg)
  {
    return alias_ptr_arg(arg);
  }
}
