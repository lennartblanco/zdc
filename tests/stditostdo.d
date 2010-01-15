/* 
 * A simple program that copies characters from stdin to stdout until
 * EOF is received.
 */

extern (C) int getchar();
extern (C) int putchar(int x);

int
main()
{
  int x = getchar();

  while ( x != -1 )
  {
    putchar(x);
    x = getchar();
  }

  return 0;
}
