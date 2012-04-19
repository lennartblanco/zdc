/** test implicitly imported definition */

extern (C) :

/* test string alias */
uint
string_tst(bool testflag)
{
  string str;

  if (testflag)
  {
    str = "oranges";
  }

  return str.length;
}

