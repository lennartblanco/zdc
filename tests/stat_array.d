//
// Test static array operations
//

// static int array, initilized with array literal expression
int init_int_array(int i)
{
    int[4] arr = [10, 11, 12, 13];

    return arr[i];
}

// static bool array, initilized with array literal expression
bool init_bool_array(int i)
{
    bool[4] arr = [true, true, false, false];

    return arr[i];
}

// static int array, with default initialization
int def_init_int_array(int i)
{
    int[6] arr;

    return arr[i];
}

// static bool array, with default initialization
bool def_init_bool_array(int i)
{
    bool[8] arr;

    return arr[i];
}

int init_exp_tst(int l)
{
    // check various init expressions
    int m = 15;
    int r = l;
    int[4] a = [l,2,3,4];

    return (a[0] + a[1] - m) * l;
}

int intops(int val, int index)
{
    // operations on integer array
    int[3] arr;
    int sum;

    arr[0] = val;
    arr[1] = 20;
    arr[2] = 30;

    sum = arr[0] + arr[1] + arr[2];

    return sum + arr[index];
}

bool boolops(bool a, bool b, bool c, int index)
{
    // operation on boolean array
    bool[4] l;
    l[0] = true;
    l[2] = b;
    l[1] = a;
    l[3] = c;

    return l[0] && l[index];
}

// equivalent to boolops() but in different code style
bool boolops2(bool a, bool b, bool c, int index)
{
    bool[4] l = [true, a, b, c];

    return l[0] && l[index];
}

// test array constant assigment to the shorthand array slice (b[])
int slice_assig_sum()
{
    int[3] b;

    b[] = [1,2,3];

    return b[0] + b[1] + b[2];
}

// test boolean array literal to shorthand array slice assigment (xs[])
bool bslice_assig(int i)
{
  bool[5] xs;

  xs[] = [true, false, true, true, false];

  return xs[i];
}

// test assigning array literals to array slices with static indexes
int slices_ops(int last_val)
{
    int[4] m;

    m[0..4] = [1 + 4, 1, -100, -1000];
    m[2..4] = [slice_assig_sum(), last_val];

    return m[0] + m[1] - m[2] - m[3];
}

// tests assigment of array slice to a shothand array slice
int
int_slice_to_shorthand_slice(int start)
{
    int[6] m;
    int[3] r;

    m[] = [9, 7, 6, 5, 4, 3];

    r[] = m[start..start+3];

    return r[0] + r[1] + r[2];
}

// tests assigment of array slice to array slice
int
int_slice_to_slice(int start)
{
    int[6] m = [15, 24, -33, 42, 51, 60];
    int[3] r;

    r[1..3] = m[start..start+2];

    return r[0] + r[1] + r[2];
}

// test assigment of boolean array slice to array slice
bool
bool_slice_to_slice(bool arg)
{
    bool[3] m = [true, true, arg];
    bool[3] r;

    r[0..3] = m[0..3];

    return r[0] && r[1] && r[2];
}

uint
uint_slice_to_slice_idx(int start)
{
    uint[6] m;
    uint[3] r;

    m[] = [3, 1, 4, 1, 5, 9];

    r[0..3] = m[start..start+3];

    return r[0] + r[1] + r[2];
}

////
//// test static array as a
//// function parameter
////
//int sum_stat(int[3] s)
//{
//    return s[0] + s[1] + s[2];
//}

//int
//call_sum_stat()
//{
//    int[3] l = [1, 2, 3];
//    int[3] m;

//    m[0] = -6;
//    m[1] = 6;
//    m[2] = 7;


//   return sum_stat(m[]) + sum_stat(l[]);
//}

////
//// test static array as last function parameter
////
//int
//stat_array_last_arg(int x, int[3] y)
//{
//  return y[x];
//}

//int
//run_stat_array_last_arg()
//{
//   int[7] arr = [10, 11, 12, 13, 14, 15, 16];
//   int[3] tst = [20, 30, 40];
//   int res;
//   int i;

//   res = stat_array_last_arg(0, arr[0..3]);
//   i = 2;
//   res = res + stat_array_last_arg(1, arr[i..i+3]);
//   res = res * stat_array_last_arg(2, tst[]);

//   return res;
//}

////
//// test static array as last function parameter
////
//int
//stat_array_first_arg(int[3] y, int mag, bool invert)
//{
//  int sum = 0;

//  sum = y[0] + y[1] + y[2];

//  if (invert) { sum = 0 - sum; };

//  return sum * mag;
//}

//int
//run_stat_array_first_arg()
//{
//  int r;
//  int[3] nums = [-20, -30, -40];

//  return stat_array_first_arg(nums[], 1, true) +
//         stat_array_first_arg(nums[0..3], 10, false);
//}

////
//// test boolean static array as function parameter
////
//int
//stat_array_bool(bool[4] s)
//{
//  int i = 0;
//  int res = 0;

//  while (i < 4)
//  {
//     if (s[i])
//     {
//       res = res + 1;
//     }
//     i = i + 1;
//  }

//  return res;
//}

//int
//run_stat_array_bool(bool big_sum)
//{
//   int r = -99;
//   bool[4] sarray = [false, true, false, true];
//   bool[8] larray = [false, false, false, false, true, true, true, false];

//   if (big_sum)
//   {
//       r = (stat_array_bool(sarray[]) +
//            stat_array_bool(larray[1..5])) *
//            stat_array_bool(larray[4..8]);
//   }
//   else
//   {
//       r = stat_array_bool(larray[0..4]);
//   }

//   return r;
//}

////
//// test assigment of integer scalar to array slice
//int
//scalar_to_arry_slice_assig(int l, int r)
//{
//    int[6] arr = l;

//    arr[3..6] = r;

//    return arr[0] + arr[1] + arr[2] + arr[3] + arr[4] + arr[5];
//}

//
// test assigment of a static integer array from a single boolean variable
//
int
stat_array_bool_var_init(int idx, int val, bool init)
{
    int[4] la = init;

    la[idx] = val;

    return la[3] + la[2] + la[1] + la[0];
}

//
// test implicit conversation of '0' and '1' in boolean array literal
// test implicit conversation of bool[4] array literal to int[4] static array
int implicit_arry_lit_casts(int i)
{
    int[4] arr = [false, 0, 0, true];

    return arr[i];
}

//
// test array slice assigment with length property
//
uint stat_array_slice_assigment_length(uint idx)
{
  uint[4] arr = [0, 0, 0, 0];
  arr[2..arr.length] = [3, 4];

  return arr[idx];
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_init_int_array(int arg)
  {
    return init_int_array(arg);
  }

  bool
  call_init_bool_array(int arg)
  {
    return init_bool_array(arg);
  }

  int
  call_def_init_int_array(int arg)
  {
    return def_init_int_array(arg);
  }

  bool
  call_def_init_bool_array(int arg)
  {
    return def_init_bool_array(arg);
  }

  int
  call_init_exp_tst(int arg)
  {
    return init_exp_tst(arg);
  }

  int
  call_intops(int arg1, int arg2)
  {
    return intops(arg1, arg2);
  }

  bool
  call_boolops(bool arg1, bool arg2, bool arg3, int arg4)
  {
    return boolops(arg1, arg2, arg3, arg4);
  }

  bool
  call_boolops2(bool arg1, bool arg2, bool arg3, int arg4)
  {
    return boolops2(arg1, arg2, arg3, arg4);
  }

  int
  call_slice_assig_sum()
  {
    return slice_assig_sum();
  }

  bool
  call_bslice_assig(int arg)
  {
    return bslice_assig(arg);
  }

  int
  call_slices_ops(int arg)
  {
    return slices_ops(arg);
  }

  int
  call_int_slice_to_shorthand_slice(int arg)
  {
    return int_slice_to_shorthand_slice(arg);
  }

  int
  call_int_slice_to_slice(int arg)
  {
    return int_slice_to_slice(arg);
  }

  bool
  call_bool_slice_to_slice(bool arg)
  {
    return bool_slice_to_slice(arg);
  }

  uint
  call_uint_slice_to_slice_idx(int arg)
  {
    return uint_slice_to_slice_idx(arg);
  }

  int
  call_stat_array_bool_var_init(bool arg1, bool arg2, bool arg3)
  {
    return stat_array_bool_var_init(arg1, arg2, arg3);
  }

  int
  call_implicit_arry_lit_casts(int arg)
  {
    return implicit_arry_lit_casts(arg);
  }

  uint
  call_stat_array_slice_assigment_length(uint arg)
  {
    return stat_array_slice_assigment_length(arg);
  }
}
