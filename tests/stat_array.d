//
// Test static array operations
//
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
