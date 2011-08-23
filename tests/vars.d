//
// Test that uninitialized variables are correctly set to
// default initializer value
//

bool uninit_bool()
{
    bool _x;

    return _x;
}

char uninit_char()
{
    char x;

    return x;
}

int uninit_int()
{
    int x;

    return x;
}

uint uninit_uint()
{
    uint x;

    return x;
}

//
// Test parsing of variouses syntaxes for local variable declarations
//
extern (C) int var_decls(int idx)
{
  int a1, a2;
  int b1 = 1, b2;
  int c1 = 12, c2 = 10;
  int d1, d2 = 2, d3;
  int e1;
  int e2 = 100;

  if (idx == 1)
  {
    return a1 + b1 + c1 + d1 + e1;
  }
  else if (idx == 2)
  {
    return a2 + b2 + c2 + d2 + e2;
  }
  else if (idx == 3)
  {
    return d3;
  }

  return -1;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
    bool
    call_uninit_bool()
    {
      return uninit_bool();
    }

    char
    call_uninit_char()
    {
      return uninit_char();
    }

    int
    call_uninit_int()
    {
      return uninit_int();
    }

    uint
    call_uninit_uint()
    {
      return uninit_uint();
    }
}
