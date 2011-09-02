//todo: add test on foreach with an array variable as aggregate

int foreach_slice()
{
    int[6] rofl = [0,1,2,3,4,5];
    int res = 0;
    int last_i;

    foreach (int i, int v; rofl[1..4]) {
      res = res + v;
      last_i = i;
    }

    return res + last_i;
}

int foreach_slice_params(int s, int e)
{
    int[6] rofl = [10, 11, 12,
                   13, 14, 15];
    int res = 0;

    foreach (int i, int v; rofl[s..e]) {
      res = res + v + i;
    }

    return res;
}

int foreach_shorthand_slice(int idx, int val)
{
    int[8] arr =
        [2, 4, 6, 8, 10, 12, 14, 16];    
    int r = 1;

    arr[idx] = val;

    foreach (int v; arr[])
    {
        r = r * v;
    }
    return r;
}

bool foreach_value_auto_type(int l)
{
   bool res = false;
   int[5] arr = [3, 4, 15, -3, 4];

   foreach (v; arr[])
   {
       res = res || v > l;
   }

   return res;
}

int foreach_index_auto_type(int needle, int[] haystack)
{
   int pos = -1;

   foreach (i, int v; haystack[])
   {
       if (v == needle)
       {
           pos = i;
       }
   }
   return pos;
}

//
// wrapper for calling foreach_index_auto_type()
//
int run_foreach_index_auto_type(int needle)
{
  int[6] arry = [97,3,211,245,128,36];

  return foreach_index_auto_type(needle, arry[]);
}

int
foreach_body_loc_vars(uint[] idxs)
{
    int sum;
    int[16] nums = 
        [19341, 06612, 48352, 49081, 23112,  28360, 41631, 62064,
         43166, 41808, 4188, 59660, 37889,  38528, 56732, 41092];

   foreach (nums_idx; idxs[])
   {
     int nums_val;

     if (nums_idx > 15) {
         return -1;
     }
     nums_val = nums[nums_idx];
     sum = sum + nums_val;
   }

   return sum;
}

uint
foreach_bool_arry(bool[] add_op)
{
   uint res;
   uint[6] vals = [11, 12, 13, 14, 15, 16];

   foreach (i, op; add_op[])
   {
      if (op) { res = res + vals[i]; }
   }   

   return res;
}

// test break statment inside a foreach
extern (C)
uint foreach_break(int num)
{
  int[] arr = [2, 7, 1, 8];
  uint cntr = 0;

  foreach (n; arr)
  {
    if (n == num)
    {
      break;
    }

    cntr = cntr + 1;
  }
  return cntr;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_foreach_slice()
  {
    return foreach_slice();
  }

  int
  call_foreach_slice_params(int arg1, int arg2)
  {
    return foreach_slice_params(arg1, arg2);
  }

  int
  call_foreach_shorthand_slice(int arg1, int arg2)
  {
    return foreach_shorthand_slice(arg1, arg2);
  }

  bool
  call_foreach_value_auto_type(int arg)
  {
    return foreach_value_auto_type(arg);
  }

  int
  call_run_foreach_index_auto_type(int arg)
  {
    return run_foreach_index_auto_type(arg);
  }

  int
  call_foreach_body_loc_vars(uint testno)
  {
    if (0 == testno)
    {
      return foreach_body_loc_vars([0, 0, 0, 0]);
    }
    else if (1 == testno)
    {
      return foreach_body_loc_vars([3, 5, 4, 2]);
    }
    else if (2 == testno)
    {
      return foreach_body_loc_vars([12, 13, 14, 15]);
    }
    else if (3 == testno)
    {
      return foreach_body_loc_vars([3, 18, 1, 0]);
    }

    return -2;
  }

  uint
  call_foreach_bool_arry(uint testno)
  {
    if (0 == testno)
    {
      return foreach_bool_arry([false, false, false, false, false, false]);
    }
    else if (1 == testno)
    {
      return foreach_bool_arry([true, true, true, true, true, true]);
    }
    else if (2 == testno)
    {
      return foreach_bool_arry([false, false, true, false, false, true]);
    }
    else if (3 == testno)
    {
      return foreach_bool_arry([true, false, false, true, false, true]);
    }
    else if (4 == testno)
    {
      return foreach_bool_arry([true, false, true, false, false, false]);
    }

    return -1;
  }
}
