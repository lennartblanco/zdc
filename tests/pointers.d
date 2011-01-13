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

// test using pointer dereference operation in expressions
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
