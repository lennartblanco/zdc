//
// Test conditional expressions, e.g. exp1 ? exp2 : exp3
//

extern (C)
{
  int abs(int i)
  {
    return i > 0 ? i : 0 - i;
  }
}
