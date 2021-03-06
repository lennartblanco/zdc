//
// Test character literals expression
//
char char_assigment()
{
  char x;

  x = 'X';

  return x;
}

char char_init()
{
  char x = 'l';

  return x;
}

char char_return()
{
  return 'r';
}

char char_array_literal(uint idx)
{
  char[] array =
      ['T', 'o', ' ', 'c', 'o', 'p', 'y', ' ', 'o', 'r', ' ', 'n', 'o', 't',
       ' ', 't', 'o', ' ', 'c', 'o', 'p', 'y', '?',
      // test escpare character expressions
       '\'', '\"', '\?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v', '\0'];


  return array[idx];
}

int my_find(char[] s, char c)
{
  uint i = 0;

  while (i < s.length) {
    if (s[i] == c) {
      return i;
    }
    i = i + 1;
  }

  return -1;
}

int invoke_array_var_find(char c)
{
  char[] s = ['h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'];

  return my_find(s, c);
}

int invoke_array_lit_find(char c)
{
  return my_find(['e', 'x', 'p', 'r', 'e', 's', 's', 'i', 'o', 'n'], c);
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  char
  call_char_assigment()
  {
    return char_assigment();
  }

  char
  call_char_init()
  {
    return char_init();
  }

  char
  call_char_return()
  {
    return char_return();
  }

  char
  call_char_array_literal(uint arg)
  {
    return char_array_literal(arg);
  }

  int
  call_invoke_array_var_find(char arg)
  {
    return invoke_array_var_find(arg);
  }

  int
  call_invoke_array_lit_find(char arg)
  {
    return invoke_array_lit_find(arg);
  }
}
