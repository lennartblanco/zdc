int abs(int a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
}

int
if_else_test(int x)
{
    int res = -1;

    if (x < 10)
    {
        res = 1;
    }
    else if (x == 10)
    {
        res = 2;
    }
    else if (5 == x)
    {
        res = 3;
    }
    else // x > 10
    {
        res = 4;
    }

    return res;
}

//void foo()
//{
//    // where does the else belong ?
//    if (true)
//        if (1 < 2)
//        {
//        }
//    else
//    {
//    }
//}
