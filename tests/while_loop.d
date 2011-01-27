int
while_tst1(int x)
{
    int res = 0;
    while (res < 20)
    {
        res = res + x;
    }

    return res;
}

int
while_tst2(int x)
{
    int res = 0;
    while (res < 20)
        res = res + x;

    return res;
}

//
// test a local variable in while-loops body scope
//
uint
while_loc_vars(int idx, uint step)
{
  uint i;

  while (i < idx)
  {
    uint tmp;

    tmp = i;
    i = tmp + step;
  }

  return i;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_while_tst1(int arg)
  {
    return while_tst1(arg);
  }

  int
  call_while_tst2(int arg)
  {
    return while_tst2(arg);
  }

  uint
  call_while_loc_vars(int arg1, uint arg2)
  {
    return while_loc_vars(arg1, arg2);
  }
}
