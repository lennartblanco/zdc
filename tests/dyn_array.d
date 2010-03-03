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

int dyn_array_sum(int[] array)
{
  uint idx = 0;
  int res;

  while (idx < array.length)
  {
    res = res + array[idx];
    idx = idx + 1;
  }

  return res;
}

/*
 * Wrapper function to test calling dyn_array_sum() with dynamic array handle
 */
int invoke_dyn_array_sum_handle(uint test_num)
{
  int[] array;

  if (test_num == 0)
  {
    array = [1, 10, 15, 0, 5];
  }
  else if (test_num == 1)
  {
    array = [5, -12];
  }
  else if (test_num == 2)
  {
    array = [0, 0, 0, 0];
  }
  else
  {
    return -1;
  }

  return dyn_array_sum(array);
}

/*
 * Wrapper function to test calling dyn_array_sum() with array literal
 */
int invoke_dyn_array_sum_lit(uint test_num)
{
  if (test_num == 0)
  {
    return dyn_array_sum([3, 1, 4, 1, 5]);
  }
  else if (test_num == 1)
  {
    return dyn_array_sum([50, 23]);
  }
  else if (test_num == 2)
  {
    return dyn_array_sum([0, 0, 0, 0]);
  }
  return -1;
}