/* 
 * A simple program that copies characters from stdin to stdout until
 * EOF is received.
 */

extern (C) int getchar();
extern (C) int putchar(int x);

void
main()
{
  int x;

  while ((x = getchar()) != -1)
  {
    putchar(x);
  }
}
