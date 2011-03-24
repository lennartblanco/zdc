//
// Test post-increment, post-decrement, pre-increment and
// pre-decrement operators
//

enum ops
{
  pre_inc,
  post_inc,
  pre_dec,
  post_dec
}

extern (C)
{
  // ++/-- on integer variable
  int
  i_ops(ops op, int var, int *var_res)
  {
    int res;

    if (ops.pre_inc == op)
    {
       res = ++var;
    }
    else if (ops.pre_dec == op)
    {
       res = --var;
    }
    else if (ops.post_inc == op)
    {
       res = var++;
    }
    else if (ops.post_dec == op)
    {
       res = var--;
    }

    *var_res = var;
    return res;
  }

  // ++/-- on uint pointer variable
  uint *
  uint_ptr_ops(ops op, uint *ptr, bool return_ptr)
  {
    uint *res;

    if (ops.pre_inc == op)
    {
       res = ++ptr;
    }
    else if (ops.pre_dec == op)
    {
       res = --ptr;
    }
    else if (ops.post_inc == op)
    {
       res = ptr++;
    }
    else if (ops.post_dec == op)
    {
       res = ptr--;
    }

    if (return_ptr)
    {
      return ptr;
    }

    return res;
  }

  // ++/-- on char pointer variable
  char *
  char_ptr_ops(ops op, char *ptr, bool return_var)
  {
    char *var = ptr;
    char *res;

    if (ops.pre_inc == op)
    {
       res = ++var;
    }
    else if (ops.pre_dec == op)
    {
       res = --var;
    }
    else if (ops.post_inc == op)
    {
       res = var++;
    }
    else if (ops.post_dec == op)
    {
       res = var--;
    }

    if (return_var)
    {
      return var;
    }

    return res;
  }
}

// test using post-increment in a loop
int arry_sum(int[] arr)
{
    uint i;
    uint res;

    while (i < arr.length)
    {
        res = res + arr[i++];
    }

    return res;
}

// test stub for calling arry_sum()
extern (C)
int
invoke_arry_sum(int array_num)
{
  int[] arr = [5, 8, 2];
  int[] empty;

  if (array_num == 0)
  {
    return arry_sum(arr);
  }
  else if (array_num == 1)
  {
    return arry_sum([3, 1, 4, 1, 5]);
  }
  else if (array_num == 2)
  {
    int[] l = [0, 0, 10, 0, -2];

    return arry_sum(l);
  }

  return arry_sum(empty);
}
