/**
 * Test sizeof property on variables
 */
uint int_var_sizeof()
{
    int x;

    return x.sizeof;
}

uint uint_var_sizeof()
{
    uint x;

    return x.sizeof;
}

uint bool_var_sizeof()
{
    bool x;

    return x.sizeof;
}

uint char_var_sizeof()
{
    char x;

    return x.sizeof;
}

uint stat_int_arry_var_sizeof()
{
    int[4] x;

    return x.sizeof;
}

uint stat_bool_arry_var_sizeof()
{
    bool[7] x;

    return x.sizeof;
}

uint dyn_int_arry_var_sizeof()
{
   int[] x;

   return x.sizeof;
}

/**
 * Test sizeof property on expressions
 */
uint int_exp_sizeof()
{
    int x;

    return (x + 1).sizeof;
}

uint bool_exp_sizeof()
{
   bool x;

   return (x && false).sizeof;
}

/**
 * Test init property on variables
 */
int int_var_init()
{
    int x;

    return x.init;
}

uint uint_var_init()
{
  uint x;

  return x.init;
}

bool bool_var_init()
{
   bool x;

   return x.init;
}

char char_var_init()
{
   char x;

   return x.init;
}

/**
 * Test length property on variables
 */
uint stat_int_arry_length()
{
    int[4] x;

    return x.length;
}

uint stat_bool_arry_length()
{
    bool[12] x;

    return x.length;
}

uint dyn_int_arry_length(bool assign)
{
    int[] x;

    if (assign)
    {
        x = [1, 2, 3, 4, 5];
    }

    return x.length;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  uint
  call_int_var_sizeof()
  {
    return int_var_sizeof();
  }

  uint
  call_uint_var_sizeof()
  {
    return uint_var_sizeof();
  }

  uint
  call_bool_var_sizeof()
  {
    return bool_var_sizeof();
  }

  uint
  call_char_var_sizeof()
  {
    return char_var_sizeof();
  }

  uint
  call_int_exp_sizeof()
  {
    return int_exp_sizeof();
  }

  uint
  call_bool_exp_sizeof()
  {
    return bool_exp_sizeof();
  }

  uint
  call_stat_int_arry_var_sizeof()
  {
    return stat_int_arry_var_sizeof();
  }

  uint
  call_stat_bool_arry_var_sizeof()
  {
    return stat_bool_arry_var_sizeof();
  }

  uint
  call_dyn_int_arry_var_sizeof()
  {
    return dyn_int_arry_var_sizeof();
  }

  int
  call_int_var_init()
  {
    return int_var_init();
  }

  uint
  call_uint_var_init()
  {
    return uint_var_init();
  }

  bool
  call_bool_var_init()
  {
    return bool_var_init();
  }

  char
  call_char_var_init()
  {
    return char_var_init();
  }

  uint
  call_stat_int_arry_length()
  {
    return stat_int_arry_length();
  }

  uint
  call_stat_bool_arry_length()
  {
    return stat_bool_arry_length();
  }

  uint
  call_dyn_int_arry_length(bool arg)
  {
    return dyn_int_arry_length(arg);
  }
}
