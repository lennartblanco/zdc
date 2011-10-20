bool int0_to_bool()
{
    bool res = 0;

    return res;
}

bool int1_to_bool()
{
    bool res = 1;

    return res;
}

int bool_to_int(bool b)
{
    int res = b;

    return res;
}

char bool_to_char(bool b)
{
    char c = b;

    return c;
}

int add_bools(bool a, bool b)
{
    return 2 * a + b;
}

int char_to_int(char c)
{
    int res = c;

    return res;
}

int char_lit_to_int()
{
    int res = 'h';

    return res;
}

uint char_to_uint(char c)
{
    uint res = c;

    return res;
}

uint char_lit_to_uint()
{
    uint res = 'Z';

    return res;
}

extern (C) int
misc_to_int(uint i)
{
  int res = -1;

  if (i == 0)
  {
    byte v = 3;

    res = v;
  }
  else if (i == 1)
  {
    ubyte v = 1;

    res = v;
  }
  else if (i == 2)
  {
    short v = 4;

    res = v;
  }
  else if (i == 3)
  {
    ushort v = 1;

    res = v;
  }
  else if (i == 4)
  {
    uint v = 5;

    res = v;
  }

  return res;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_bool_to_int(bool arg)
  {
      return bool_to_int(arg);
  }

  char
  call_bool_to_char(bool arg)
  {
      return bool_to_char(arg);
  }

  bool
  call_int0_to_bool()
  {
      return int0_to_bool();
  }

  bool
  call_int1_to_bool()
  {
      return int1_to_bool();
  }

  int
  call_add_bools(bool arg1, bool arg2)
  {
      return add_bools(arg1, arg2);
  }

  int
  call_char_to_int(char arg1)
  {
      return char_to_int(arg1);
  }

  int
  call_char_lit_to_int()
  {
      return char_lit_to_int();
  }

  uint
  call_char_to_uint(char arg1)
  {
      return char_to_uint(arg1);
  }

  uint
  call_char_lit_to_uint()
  {
      return char_lit_to_uint();
  }
}
