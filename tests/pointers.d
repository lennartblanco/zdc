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
