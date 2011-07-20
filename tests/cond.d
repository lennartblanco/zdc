//
// Test conditional expressions, e.g. exp1 ? exp2 : exp3
//

extern (C)
{
  int abs(int i)
  {
    return i > 0 ? i : 0 - i;
  }

  int const_cond(int n)
  {
    if (n == 0)
    {
      return true ? 100 : -1;
    }
    else if (n == 1)
    {
      return false ? -2 : 200;
    }
    else if (n == 2)
    {
      return 3 < 10 ? 300 : -3;
    }
    else if (n == 3)
    {
      return 'x' == 'b' ? -4 : 400;
    }
    return 0;
  }
}
