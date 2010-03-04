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
       ' ', 't', 'o', ' ', 'c', 'o', 'p', 'y', '?'];

  return array[idx];
}


