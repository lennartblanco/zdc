// A comment!

int sum(int first, int second, int third)
{
    return first + second + third;
}

int subst3(int first, int second, int third)
{
    return first - second - third;
}

int subst4(int first, int second, int third, int fourth)
{
    return first - second - third - fourth;
}



int add(int a, int b)
{
    return a+b;
}

int
get_13()
{
    return 13;
}

int foo()
{
    get_13();
    return get_13() + 1;
}

int hej()
{
    return add(10, foo());
}

int ind_sum(int l)
{
    return sum(l, get_13(), 10);
}

// test function definition with first unnamed parameter
int unnamed_arg1(int, int x)
{
  return x;
}

// test function definition with second unnamed parameter
int unnamed_arg2(int x, int)
{
  return x;
}

// test function definition with last unnamed parameter
int unnamed_arg3(int a, bool b, uint)
{
  if (b) { return -a; }
  return a;
}

// test function definition with all parameter unnamed
int unnamed_arg4(int, int, int)
{
  return 10;
}

// function call in variable initilization expression
int add_13(int i)
{
    int x = get_13();

    return x + i;
}

// test calling a function (subst3) with 3 arguments
int invoke_subst3(int x)
{
    return subst3(x, 10, 20);
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_hej()
  {
    return hej();
  }

  int
  call_get_13()
  {
    return get_13();
  }

  int
  call_foo()
  {
    return foo();
  }

  int
  call_add(int arg1, int arg2)
  {
    return add(arg1, arg2);
  }

  int
  call_sum(int arg1, int arg2, int arg3)
  {
    return sum(arg1, arg2, arg3);
  }

  int
  call_ind_sum(int arg)
  {
    return ind_sum(arg);
  }

  int
  call_subst3(int arg1, int arg2, int arg3)
  {
    return subst3(arg1, arg2, arg3);
  }

  int
  call_subst4(int arg1, int arg2, int arg3, int arg4)
  {
    return subst4(arg1, arg2, arg3, arg4);
  }

  int
  call_add_13(int arg)
  {
    return add_13(arg);
  }

  int
  call_invoke_subst3(int arg)
  {
    return invoke_subst3(arg);
  }

  int
  call_unnamed_arg1(int arg1, int arg2)
  {
    return unnamed_arg1(arg1, arg2);
  }

  int
  call_unnamed_arg2(int arg1, int arg2)
  {
    return unnamed_arg2(arg1, arg2);
  }

  int
  call_unnamed_arg3(int arg1, bool arg2, uint arg3)
  {
    return unnamed_arg3(arg1, arg2, arg3);
  }

  int
  call_unnamed_arg4(int arg1, int arg2, int arg3)
  {
    return unnamed_arg4(arg1, arg2, arg3);
  }
}
