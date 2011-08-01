extern (C)
{

int
for_tst1(int start, int stop)
{
  int r;

  /* test vanilla for loop */

  for (int i = start; i < stop ; i++)
  {
    r = r + 1;
  }

  return r;
}

int
for_tst2(int init, int min)
{
  int i = init;
  int steps;

  /* test for loop without init code */
  for (; i > min; --i)
  {
    steps = steps + 1;
  }

  return steps;
}

int
for_tst3(int step)
{
  int cntr = 0;

  /* test for loop without any step expression */
  for (int i = 0; i < 10 ;)
  {
    i = i + step;
    cntr = cntr + 1;
  }

  return cntr;
}




int
for_div(int a, int b)
{
  int i;

  /* vanilla for loop 2 */
  for (i = 0; a >= b; i++)
  {
    a = a - b;
  }

  return i;
}

int
for_break(int start)
{
  int i;

  /* test for loop without test expression and test break from for loop */
  for (i = start; ; i++)
  {
    if ((i % 5) == 0)
    {
        break;
    }
  }

  return i;
}

}

uint
for_tst4(bool[] arr)
{
  /* test for loop without init code, without test and step expressions */
  uint i = 0;
  for (;;)
  {
     if (i >= arr.length || arr[i]) {
       break;
     }
     i = i + 1;
  }

  return i;
}

extern (C) int
invoke_for_tst4(int test_case)
{
  if (0 == test_case)
  {
    return for_tst4([false]);
  }
  else if (1 == test_case)
  {
    return for_tst4([true]);
  }
  else if (2 == test_case)
  {
    return for_tst4([false, false, false]);
  }
  else if (3 == test_case)
  {
    return for_tst4([false, false, true, true, false]);
  }

  return -1;
}
