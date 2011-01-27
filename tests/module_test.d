// test parsing of module specification
// and function symbols mangling
module trinity;

void
depl(uint c)
{
  boom(c, 2);
}

bool
boom(int b, int a)
{
  return (b - a) == 0;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  void
  call_depl(uint arg)
  {
    depl(arg);
  }

  bool
  call_boom(int arg1, int arg2)
  {
    return boom(arg1, arg2);
  }
}
