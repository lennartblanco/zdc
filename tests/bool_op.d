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
