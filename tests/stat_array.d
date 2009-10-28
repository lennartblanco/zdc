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
    l[1] = a;
    l[2] = b;
    l[3] = c;

    return l[0] && l[index];
}

// equivalent to boolops() but in different code style
bool boolops2(bool a, bool b, bool c, int index)
{
    bool[4] l = [true, a, b, c];

    return l[0] && l[index];
}

// test array constant assigment to the shorthand array slice (foo[])
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

// test assigning to array slices with static indexes
int slices_ops(int last_val)
{
    int[4] m;

    m[0..4] = [1 + 4, 1, -100, -1000];
    m[2..4] = [slice_assig_sum(), last_val];

    return m[0] + m[1] - m[2] - m[3];
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


//   return sum_stat(m) + sum_stat(l);
//}

////
//// test assigment of a static integer array from single integer constant
////
//// test cases:
//// stat_array_int_const_init(0, 102) = 62
//// stat_array_int_const_init(2, 2) = -22
//// stat_array_int_const_init(4, -100) = 80
////
//int
//stat_array_int_const_init(int idx, int val)
//{
//    int[5] ar = 10;

//    ar[idx] = val;

//    return ar[0] - ar[1] - ar[2] - ar[3] - ar[4];
//}

////
//// test assigment of a static integer array from a single boolean variable
////
//// test cases:
//// stat_array_bool_var_init(3, 100, true) = 103
//// stat_array_bool_var_init(2, 20, false) = 20
//// stat_array_bool_var_init(0, 0, true) = 3
////
//int
//stat_array_bool_var_init(int idx, int val, bool init)
//{
//    int[4] la = init;

//    la[idx] = val;

//    return la[3] + la[2] + la[1] + la[0];
//}

////
//// test implicit conversation of '0' and '1' in boolean array literal
//// test implicit conversation of bool[4] array literal to int[4] static array 
//int implicit_arry_lit_casts(int i)
//{
//    int[4] arr = [false, 0, 0, true];

//    return arr[i];
//}

