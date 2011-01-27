// test parsing of module specification
// and function symbols mangling
module a.test.pkg.name;

int first_func()
{
  return 1969;
}

bool second_func(int b)
{
  return b == false;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_first_func()
  {
    return first_func();
  }

  bool
  call_second_func(int arg)
  {
    return second_func(arg);
  }
}
