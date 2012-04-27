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

char char_array(char val, uint indx)
{
  /* test reading and writing to array cells with 8-bit elements */
  char[] arr = ['\0', 'b', '\0', 'd', 'e', '\0'];

  arr[0] = val;
  arr[2] = 'c';
  arr[arr.length - 1] = val;

  return arr[indx];
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

bool find_int(int[] haystack, int needle)
{
  while (haystack.length > 0)
  {
    if (haystack[0] == needle)
    {
       return true;
    }
    haystack = haystack[1..haystack.length];
  }

  return false;
}


int dyn_array_slice_assigment(bool assign, uint idx)
{
  int[] arr = [1, 2, 3, 4, 5];

  if (assign)
  {
      arr[2..4] = [11, 22];
  }

  return arr[idx];
}

uint dyn_array_slice_assigment_length(uint idx)
{
  uint[] arr = [0, 0, 0, 0, 0, 0];
  arr[3..arr.length] = [99, 98, 97];

  return arr[idx];
}

int dyn_array_slice_shorthand(bool assign, uint idx)
{
  int[] arr = [-1, -1, -1, -1, -1];

  if (assign)
  {
      arr[] = [11, 22, 33, 44, 55];
  }

  return arr[idx];
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

/*
 * Wrapper function to test calling find_int()
 */
bool invoke_find_int(uint test_num)
{
  int[] array = [1, 2, 3, 400, 5, 6];

  if (test_num == 0)
  {
     return find_int([3, 1, 4, 7], 4);
  }
  else if (test_num == 1)
  {
     return find_int([3, 1, 4, 7], 20);
  }
  else if (test_num == 2)
  {
    return find_int(array, 100);
  }
  else if (test_num == 3)
  {
    return find_int(array[1..3], 1);
  }
  else if (test_num == 4)
  {
    return find_int(array[1..4], 400);
  }
}

extern (C) :

/**
 * test assignment of 'null' to array variable
 */
uint
array_null_assign(bool assign_null)
{
  byte[] l = [1, 2, 3, 4];

  if (assign_null) {
    l = null;
  }

  return l.length;
}

/**
 * test using 'null' in function call for int[] argument
 */
int
array_null_arg(bool use_null)
{
  if (use_null) {
    return dyn_array_sum(null);
  }
  return dyn_array_sum([10, 2, 3]);
}

/*
 * wrappers to allow call test functions with C calling convention
 */

int
call_int_array_lit_assigment(int arg)
{
  return int_array_lit_assigment(arg);
}

int
call_int_array_lit_init(int arg)
{
  return int_array_lit_init(arg);
}

int
call_intops(int arg1, int arg2)
{
  return intops(arg1, arg2);
}

bool
call_bool_array_lit_assigment(int arg)
{
  return bool_array_lit_assigment(arg);
}

bool
call_bool_array_lit_init(bool arg)
{
  return bool_array_lit_init(arg);
}

bool
call_boolops(int arg1, int arg2)
{
  return boolops(arg1, arg2);
}

char
call_char_array(char arg1, int arg2)
{
  return char_array(arg1, arg2);
}

int
call_invoke_dyn_array_sum_handle(int arg)
{
  return invoke_dyn_array_sum_handle(arg);
}

int
call_invoke_dyn_array_sum_lit(int arg)
{
  return invoke_dyn_array_sum_lit(arg);
}

int
call_dyn_array_slice_assigment(bool arg1, uint arg2)
{
  return dyn_array_slice_assigment(arg1, arg2);
}

uint
call_dyn_array_slice_assigment_length(uint arg)
{
  return dyn_array_slice_assigment_length(arg);
}

int
call_dyn_array_slice_shorthand(bool arg1, uint arg2)
{
  return dyn_array_slice_shorthand(arg1, arg2);
}

int
call_invoke_find_int(int arg)
{
  return invoke_find_int(arg);
}
