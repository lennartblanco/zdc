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

uint
signed_unsigned_const_add()
{
  uint res;

  res = 1 + 2u;

  return res;
}

uint
unsigned_signed_const_div()
{
  uint res;

  res = 15u / 3;

  return res;
}


uint
signed_unsigned_add(int l, uint r)
{
  uint res;

  res = l + r;

  return res;
}

uint
unsigned_signed_mult(uint l, int r)
{
  uint res;

  res = l * r;

  return res;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  bool
  call_uint_assig()
  {
    return uint_assig();
  }

  bool
  call_uint_arg(uint arg)
  {
    return uint_arg(arg);
  }

  uint
  call_uint_ret(bool arg)
  {
    return uint_ret(arg);
  }

  uint
  call_uint_arrys(uint arg)
  {
    return uint_arrys(arg);
  }

  uint
  call_signed_unsigned_const_add()
  {
    return signed_unsigned_const_add();
  }

  uint
  call_unsigned_signed_const_div()
  {
    return unsigned_signed_const_div();
  }

  int
  call_signed_unsigned_add(int arg1, uint arg2)
  {
    return signed_unsigned_add(arg1, arg2);
  }

  int
  call_unsigned_signed_mult(int arg1, uint arg2)
  {
    return unsigned_signed_mult(arg1, arg2);
  }
}
