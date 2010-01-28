//
// Test dynamic array operations
//

int int_array_lit_assigment(int idx)
{
  int[] x;

  x = [3, 1, 4];

  return x[0] + x[idx];
}

int int_array_lit_init(int idx)
{
  int[] arr = [2, 7, 1, 8, 2];

  return arr[idx];
}

int intops(int idx, int v)
{
  int[] x = [1, 2, 3];
  int[10] y;

  x[idx] = v;

  return x[0] + x[1] + x[2];
}

bool bool_array_lit_assigment(int i)
{
  bool[] x;

  x = [true, false, true, false];

  return x[i];
}

bool bool_array_lit_init(bool a)
{
  bool[] x = [true, a];

  return x[0] && x[1];
}

bool boolops(int idx, int v)
{
  bool[] x = [false, false, false];

  x[idx] = v > 0;

  return x[0] || x[1] || x[2];
}
