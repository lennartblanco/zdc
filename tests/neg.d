int neg0(int arg)
{
  return -arg;
}

int neg1(int u)
{
   u = - u;

   return u;
}

int neg2(int u)
{
   u = -(u);

   return u;
}

int neg3()
{
    return -(neg2(3));
}

int neg4()
{
    return -neg2(1);
}

int neg5(int l)
{
   int v;

   v = 10 * 4 + l;

   return v;
}

int neg51(int l)
{
   int v = 10 * 4 + l;

   return v;
}

int neg6(int l)
{
   int v;

   v = 10 * (4 + neg1(l));

   return v;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_neg0(int arg)
  {
    return neg0(arg);
  }

  int
  call_neg1(int arg)
  {
    return neg1(arg);
  }

  int
  call_neg2(int arg)
  {
    return neg2(arg);
  }

  int
  call_neg3()
  {
    return neg3();
  }

  int
  call_neg4()
  {
    return neg4();
  }

  int
  call_neg5(int arg)
  {
    return neg5(arg);
  }

  int
  call_neg51(int arg)
  {
    return neg51(arg);
  }

  int
  call_neg6(int arg)
  {
    return neg6(arg);
  }
}
