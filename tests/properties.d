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



