extern (C) int
integer_promotions(uint testno, int a)
{
  int res = -1;

  /* int and char */
  if (testno == 0)
  {
    char b = 'x';
    res = a + b;
  }
  /* int and bool */
  else if (testno == 1)
  {
    bool b = true;
    res = a - b;
  }
  /* int and byte */
  else if (testno == 2)
  {
    byte b = -2;
    res = a * b;
  }
  /* int and ubyte */
  else if (testno == 3)
  {
    ubyte b = 5;
    res = a / b;
  }
  /* short and int */
  else if (testno == 4)
  {
    short b = 10;
    res = b + a;
  }
  /* ushort and int */
  else if (testno == 5)
  {
    ushort b = 100;
    res = b - a;
  }
  /* int and int */
  else if (testno == 6)
  {
    int b = 8;
    res = b * a;
  }
  /* uint and int */
  else if (testno == 7)
  {
    uint b = 200;
    res = b / a;
  }


  return res;
}
