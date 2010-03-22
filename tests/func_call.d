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
