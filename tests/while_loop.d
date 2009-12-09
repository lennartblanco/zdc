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
