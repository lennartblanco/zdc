//
// Test on various arithmetic operations, e.g. /, %
//

// todo: add tests on signed and unsigned +, -, *

// test division of two signed integer variables
int
signed_div_vars(int left, int right)
{
    return left / right;
}

// test division of two unsigned integer variables
uint
unsigned_div_vars(uint left, uint right)
{
    int a = left;

    return a / right;
}

// test division of signed constant and integer variable
int
signed_div_lconst(int right)
{
    return 60 / right;
}

// test division of unsigned integer variable and constant
uint
unsigned_div_rconst(uint left)
{
    uint res;

    res = left / 100;

    return res;
}

// test modulo operation over two constans
int
const_mod()
{
  return 49 % 10;
}

// test modulo of two signed integer variables
int
signed_mod_vars(int left, int right)
{
    return left % right;
}

// test modulo of two unsigned integer variables
uint
unsigned_mod_vars(uint left, uint right)
{
    int a = left;

    return a % right;
}

// test modulo of signed constant and integer variable
int
signed_mod_lconst(int right)
{
    return 60 % right;
}

// test modulo of unsigned integer variable and constant
uint
unsigned_mod_rconst(uint left)
{
    uint res;

    res = left % 100;

    return res;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_signed_div_vars(int arg1, int arg2)
  {
    return signed_div_vars(arg1, arg2);
  }

  uint
  call_unsigned_div_vars(uint arg1, uint arg2)
  {
    return unsigned_div_vars(arg1, arg2);
  }

  int
  call_signed_div_lconst(int arg)
  {
    return signed_div_lconst(arg);
  }

  uint
  call_unsigned_div_rconst(uint arg)
  {
    return unsigned_div_rconst(arg);
  }

  int
  call_const_mod()
  {
    return const_mod();
  }

  int
  call_signed_mod_vars(int arg1, int arg2)
  {
    return signed_mod_vars(arg1, arg2);
  }

  uint
  call_unsigned_mod_vars(uint arg1, uint arg2)
  {
    return unsigned_mod_vars(arg1, arg2);
  }

  int
  call_signed_mod_lconst(int arg)
  {
    return signed_mod_lconst(arg);
  }

  uint
  call_unsigned_mod_rconst(uint arg)
  {
    return unsigned_mod_rconst(arg);
  }
}
