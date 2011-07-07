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

//
// test nested while-loops
//
bool while_nested(char[] arr)
{
  uint i = 0, j;

  while (i < arr.length)
  {
    char c = arr[i];
    j = ++i;
    while (j < arr.length)
    {
      if (arr[j] == c)
      {
         return false;
      }
      j = j + 1;
    }
  }

  return true;
}

// test driver for while_nested()
extern (C)
bool invoke_while_nested(uint test_num)
{
    char[] a;

    if (test_num == 1)
    {
         a = ['a', 'b', 'c', 'd'];
    }
    else if (test_num == 2)
    {
         a = ['h', 'e', 'l', 'l', 'o'];
    }
    else if (test_num == 3)
    {
         a = ['X', 'a', 'b', 'c', 'd', 'e', 'X'];
    }

    return while_nested(a);
}

// test break statment inside a while-loop
extern (C) int
while_break(int break_val)
{
  int i;

  while (i < 10)
  {
    i = i + 1;
    if (i == break_val)
    {
      break;
    }
  }

  return i;
}

// test break inside nested while loop
extern (C)
int while_nested_break(bool do_break)
{
  bool loop = true;

  while (loop)
  {
    while (loop)
    {
      // check that break aborts the inner loop
      if (do_break)
      {
        break;
      }
      return 1;
    }
    loop = false;
  }

  return 2;
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
