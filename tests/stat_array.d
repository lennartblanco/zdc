//
// Test static array operations
//

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

// test assigning to array slices with static indexes
int slices_ops(int last_val)
{
    int[4] m;

    m[0..4] = [1 + 4, 1, -100, -1000];
    m[2..4] = [slice_assig_sum(), last_val];

    return m[0] + m[1] - m[2] - m[3];
}

//
// test static array as a
// function parameter
//
int sum_stat(int[3] s)
{
    return s[0] + s[1] + s[2];
}

int
call_sum_stat()
{
    int[3] l = [1, 2, 3];
    int[3] m;

    m[0] = -6;
    m[1] = 6;
    m[2] = 7;


   return sum_stat(m) + sum_stat(l);
}



