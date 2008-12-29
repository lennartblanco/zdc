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
    int x;

    if (x < 10)
    {
        return -1;
    }
    else if (x == 10)
    {
        return 1;
    }
    else if (false)
    {
        x = 2;
    }
    else // x > 10
    {
        x = 1;
    }

    return x;
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
