bool
return_true()
{
    return true;
}

bool
return_false()
{
    return false;
}

bool
is_less_or_equal(int left, 
                 int right)
{
    return left <= right;
}

bool
is_greater(int a, int b)
{
    bool res;

    res = a > b;

    return res;
}

bool
greater_tst1()
{
    bool f;

    f = 10 + 2 > 20;

    return f;
}

bool
greater_tst2(int x)
{
    bool f;

    f = 10 + 2 > 10 * x;
    return f;
}

bool
greater_tst3(int x)
{
    return 10 + 2 > 20 * x;
}

bool
is_less(int a, int b)
{
    bool res;

    res = a < b;

    return res;
}

bool
less_tst1()
{
    bool f;

    f = 10 + 2 < 20;

    return f;
}

bool
less_tst2(int x)
{
    bool f;

    f = 10 + 2 < 20 * x;
    return f;
}

bool
less_tst3(int x)
{
    return 10 + 2 < 20 * x;
}

bool
and_oper(bool left, bool right)
{
//    return left && right;
    return false;
}
