//
// Test dynamic array operations
//

int int_array_lit_assigment(int idx)
{
  int[] x;

  x = [3, 1, 4];

  return x[0] + x[idx];
}

bool bool_array_lit_assigment(int i)
{
  bool[] x;

  x = [true, false, true, false];

  return x[i];
}
