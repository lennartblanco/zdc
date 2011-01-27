import fact;
import rets, neg;

int
run_fact(bool both)
{
  int res;

  res = fact_rec(5);

  if (both) { res = res + fact_while(3); }

  return res;
}

int
run_misc()
{
  return neg0(int_ret());
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_run_fact(bool arg)
  {
    return run_fact(arg);
  }

  int
  call_run_misc()
  {
    return run_misc();
  }
}
