//todo: add tests on unsiged <, >, <=, >=, == and != operations

bool
return_true()
{
    return true;
}

bool
return_false()
{
    return false;
}

bool invert(bool arg)
{
    return !arg;
}

bool is_equal(int p0, int p1) { return p0 == p1; }

bool not_equal(int alice, int bob) 
{ 
    return alice != bob; 
}

bool
is_less_or_equal(int left, 
                 int right)
{
    return left <= right;
}

bool is_greater_or_equal(int l, int r)
{
    return l >= r;
}

bool
is_greater(int a, int b)
{
    bool res;

    res = a > b;

    return res;
}

bool
greater_tst1()
{
    bool f;

    f = 10 + 2 > 20;

    return f;
}

bool
greater_tst2(int x)
{
    bool f;

    f = 10 + 2 > 10 * x;
    return f;
}

bool
greater_tst3(int x)
{
    return 10 + 2 > 20 * x;
}

bool
is_less(int a, int b)
{
    bool res;

    res = a < b;

    return res;
}

bool
less_tst1()
{
    bool f;

    f = 10 + 2 < 20;

    return f;
}

bool
less_tst2(int x)
{
    bool f;

    f = 10 + 2 < 20 * x;
    return f;
}

bool
less_tst3(int x)
{
    return 10 + 2 < 20 * x;
}

bool
tripple_and_op(bool left, bool right, bool last)
{
   return left && right && last;
}

bool
andor_ops(bool a, bool b, bool c)
{
    return a && b || c;
}

bool
and_oper(bool left, bool right)
{
    return left && right;
}

bool
or_oper(bool l, bool r)
{
    return l || r;
}

int
nested_eq(int arg)
{
  bool a;
  bool b;
  bool c;

  a = (1 == (arg == 10)) == false;
  b = (1 == arg) == (10 == 22);
  c = 1 == ((arg == 10) == true);

  return  a+b+c;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  bool
  call_return_true()
  {
    return return_true();
  }

  bool
  call_return_false()
  {
    return return_false();
  }

  bool
  call_invert(bool arg)
  {
    return invert(arg);
  }

  int
  call_is_equal(int arg1, int arg2)
  {
    return is_equal(arg1, arg2);
  }

  int
  call_not_equal(int arg1, int arg2)
  {
    return not_equal(arg1, arg2);
  }

  int
  call_is_less_or_equal(int arg1, int arg2)
  {
    return is_less_or_equal(arg1, arg2);
  }

  int
  call_is_greater_or_equal(int arg1, int arg2)
  {
    return is_greater_or_equal(arg1, arg2);
  }

  int
  call_is_greater(int arg1, int arg2)
  {
    return is_greater(arg1, arg2);
  }

  int
  call_is_less(int arg1, int arg2)
  {
    return is_less(arg1, arg2);
  }

  bool
  call_greater_tst1()
  {
    return greater_tst1();
  }

  bool
  call_greater_tst2(int arg)
  {
    return greater_tst2(arg);
  }

  bool
  call_greater_tst3(int arg)
  {
    return greater_tst3(arg);
  }

  bool
  call_less_tst1()
  {
    return less_tst1();
  }

  bool
  call_less_tst2(int arg)
  {
    return less_tst2(arg);
  }

  bool
  call_less_tst3(int arg)
  {
    return less_tst3(arg);
  }

  bool
  call_and_oper(bool arg1, bool arg2)
  {
    return and_oper(arg1, arg2);
  }

  bool
  call_or_oper(bool arg1, bool arg2)
  {
    return or_oper(arg1, arg2);
  }

  bool
  call_tripple_and_op(bool arg1, bool arg2, bool arg3)
  {
    return tripple_and_op(arg1, arg2, arg3);
  }

  bool
  call_andor_ops(bool arg1, bool arg2, bool arg3)
  {
    return andor_ops(arg1, arg2, arg3);
  }

  int
  call_nested_eq(int arg)
  {
    return nested_eq(arg);
  }
}
