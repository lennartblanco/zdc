extern (C) int putchar(int);

void myprint(immutable(char)[] str)
{
  uint i;

  while (i < str.length)
  {
    putchar(str[i]);
    i = i + 1;
  }  
}

void main()
{
  immutable(char)[] str;

  str = "I will not buy this record;";
  myprint(str);
  myprint(" it is scratched.\n");
}




