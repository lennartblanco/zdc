//
// test using an int aliased to my_int
//
alias int my_int;

my_int
func1(int a, my_int b)
{
  my_int res;

  res = a;
  res = res + b;

  return res;
}

extern (C) int
invoke_func1(int arg1, int arg2)
{
  return func1(arg1, arg2);
}

extern (C) int
func2(my_int a)
{
  return func1(a, 2) * 3;
}

//
// test an 'string' alias
//
alias immutable(char)[] my_string;

uint
strlen(my_string str)
{
  return str.length;
}

extern (C) uint
invoke_strlen(uint test_case)
{
  if (test_case == 0)
  {
    return strlen("orange, apple, lemon");
  }
  else if (test_case == 1)
  {
    my_string str = "brown fox jumped";

    return strlen(str);
  }

  return 0;
}


