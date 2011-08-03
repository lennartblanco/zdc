//
// Test conditional expressions, e.g. exp1 ? exp2 : exp3
//

extern (C)
{
  int abs(int i)
  {
    return i > 0 ? i : 0 - i;
  }

  // test constant conditional expressions
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

  int cond_statement(bool inc)
  {
    int x = 10;

    // test using conditional expression as a statement
    inc ? x++ : x--;

    return x;
  }

  // test conditional expression of array type
  int array_cond(uint idx)
  {
    int[] a = [1, 20, 300, 4000, 50000];
    int[] r;

    r = idx < a.length ? a[0..idx] : a;

    int q;
    foreach (n; r)
    {
       q = q + n;
    }

    return q;
  }
}
