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
