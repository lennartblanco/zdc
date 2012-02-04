// factorial function, implemented with recursion
public int
fact_rec(int i)
{
    if (i <= 0)
    {
        return 1;
    }

    return i * fact_rec(i-1);
}

// factorial function, implemented with a while-loop
int
fact_while(int i)
{
    int res = 1;
    while (i > 1)
    {
        res = res * i;
        i = i - 1;
    }

    return res;
}

// factorial function, implemented with a while-loop and post-increment operand
uint
fact_while_postinc(uint i)
{
    if (i == 0) { return 1; }

    uint res = 1;
    while (i-- > 1)
    {
        res = res * (i + 1);
    }

    return res;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_fact_rec(int arg)
  {
    return fact_rec(arg);
  }

  int
  call_fact_while(int arg)
  {
    return fact_while(arg);
  }

  uint
  call_fact_while_postinc(uint arg)
  {
    return fact_while_postinc(arg);
  }
}
