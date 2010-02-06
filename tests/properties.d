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


