bool
uint_assig()
{
  uint a;
  uint b;
  uint y = 20u;
  uint z = 10U;

  a = 22u;
  b = 11U;

  return (a - b) > (y - z);
}

bool
uint_arg(uint arg)
{
  return arg == 3u;
}

uint
uint_ret(bool x)
{
  uint ret;

  if (x) { ret = 42u; }

  return ret;
}

uint
uint_arrys(uint idx)
{
  uint[4] a = [10u, 20, 30, 40];
// todo: add support for below alternative declarations of b and c arrays:
//  uint[3 + 1] b;
//  uint[4u] c;
  uint[4] b;
  uint[4] c;
  uint[4] d = [true, false, true, false];

  b[] = [1000u, 2000u, 3000u, 4000u];
  c[] = [100, 200, 300, 400];

  return a[idx] + b[idx] + c[idx] + d[idx];
}

