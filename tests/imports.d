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

