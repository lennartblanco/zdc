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
