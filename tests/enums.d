enum states // base type int
{
  idle,
  run,
  stop
}

enum ustates // base type uint
{
  on = 10u,
  off,
  disabled
}

enum nums // base type char
{
  ZERO = '0',
  ONE,
  TWO,
  FIVE = '5',
  SIX,
  SEVEN,
  EIGHT
}

enum nato : char
{
  UNKNOW,      //   0
  ABORT,       //   1
  ALPHA = 'a', //   97
  BRAVO,       //   98
  CHARLIE,     //   99
  DELTA,       //   100
  ECHO         //   101
}

enum logic // base type bool
{
  T = true,
  F = false
}

// test cases:
// states_get_def_as_int() = 0
int
states_get_def_as_int()
{
    states x;

    return x;
}

// ustates_get_def_as_int() = 10
uint
ustates_get_def_as_int()
{
    ustates x;

    return x;
}

//nums_get_def_as_char() = '0'
char
nums_get_def_as_char()
{
   nums x;

   return x;
}

//nato_get_def_as_char() = 0
char
nato_get_def_as_char()
{
  nato x;

  return x;
}

// logic_get_def_as_bool() = true
bool
logic_get_def_as_bool()
{
   logic x;

   return x;
}

