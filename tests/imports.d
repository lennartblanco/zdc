import fact;
import rets, neg;
private import hex;
import import_proxy;

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

int
run_hex_int(bool flag)
{
  return hex_int(flag ? 0 : -1);
}

int
run_imp_func1(bool flag)
{
  if (flag)
  {
    return imp_func1(314);
  }
  return imp_func1(-29);
}

/* call function imported indirectly by the import_proxy module */
bool
run_invert(bool arg)
{
  return invert(arg);
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

  int
  call_run_hex_int(bool arg)
  {
    return run_hex_int(arg);
  }

  int
  call_run_imp_func1(bool flag)
  {
    return run_imp_func1(flag);
  }

  bool
  call_run_invert(bool arg)
  {
    return run_invert(arg);
  }
}
