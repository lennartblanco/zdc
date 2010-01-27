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

