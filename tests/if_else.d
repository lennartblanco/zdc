int abs(int a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
}

int
if_else_test(int x)
{
    int res = -1;

    if (x < 10)
    {
        res = 1;
    }
    else if (x == 10)
    {
        res = 2;
    }
    else if (15 == x)
    {
        res = 3;
    }
    else // x > 10
    {
        res = 4;
    }

    return res;
}

int
if_else_less_10(int x)
{
    if (x < 10)
    {
        return -1;
    }
    else if (x > 10)
    {
        return 1;
    }
    return 0;
}

bool
if_isless(int a)
{
    if (a < 0)
    {
        return true;
    }

    return false;
}

bool
if_isgreater(int x)
{
    if (x > 0)
    {
        return true;
    }

    return false;
}

bool
if_isless_eq(int x)
{
    if (x <= 0)
    {
        return true;
    }

    return false;
}

bool
if_isgreater_eq(int x)
{
    if (x >= 0)
    {
        return true;
    }

    return false;
}

bool
if_iseq(int x, int y)
{
    if (x == y)
    {
        return true;
    }

    return false;
}

bool
if_isnoteq(int x, int y)
{
    if (x != y)
    {
        return true;
    }

    return false;
}

// return -1 if x is less then 0
//         1 if x is greater then 0
//         0 if x is 0
// (test that compiler handles function calls in if-condition properly)
int get_sign(int x)
{
    int res;
    if (if_isless(x))
    {
        res = -1;
    }
    else if (if_isgreater(x))
    {
        res = 1;
    }
    else
    {
        res = x;
    }

    return res;
}

int iret_n_arg(int n, int arg1, int arg2, int arg3)
{
   if (n == 1) return arg1;
   else if (n == 2) return arg2;
   else if (n == 3) return arg3;
   else return -1;
}

/*
 * wrappers to allow call test functions with C calling convention
 */

extern (C)
{
  bool
  call_if_isless(int arg)
  {
    return if_isless(arg);
  }

  bool
  call_if_isgreater(int arg)
  {
    return if_isgreater(arg);
  }

  bool
  call_if_isless_eq(int arg)
  {
    return if_isless_eq(arg);
  }

  bool
  call_if_isgreater_eq(int arg)
  {
    return if_isgreater_eq(arg);
  }

  bool
  call_if_iseq(int arg1, int arg2)
  {
    return if_iseq(arg1, arg2);
  }

  bool
  call_if_isnoteq(int arg1, int arg2)
  {
    return if_isnoteq(arg1, arg2);
  }

  int
  call_abs(int arg)
  {
    return abs(arg);
  }

  int
  call_if_else_test(int arg)
  {
    return if_else_test(arg);
  }

  int
  call_get_sign(int arg)
  {
    return get_sign(arg);
  }

  int
  call_iret_n_arg(int arg1, int arg2, int arg3, int arg4)
  {
    return iret_n_arg(arg1, arg2, arg3, arg4);
  }
}
