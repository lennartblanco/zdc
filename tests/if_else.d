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
    else if (15 == x)
    {
        res = 3;
    }
    else // x > 10
    {
        res = 4;
    }

    return res;
}

int
if_else_less_10(int x)
{
    if (x < 10)
    {
        return -1;
    }
    else if (x > 10)
    {
        return 1;
    }
    return 0;
}

bool
if_isless(int a)
{
    if (a < 0)
    {
        return true;
    }

    return false;
}

bool
if_isgreater(int x)
{
    if (x > 0)
    {
        return true;
    }

    return false;
}

bool
if_isless_eq(int x)
{
    if (x <= 0)
    {
        return true;
    }

    return false;
}

bool
if_isgreater_eq(int x)
{
    if (x >= 0)
    {
        return true;
    }

    return false;
}

bool
if_iseq(int x, int y)
{
    if (x == y)
    {
        return true;
    }

    return false;
}

bool
if_isnoteq(int x, int y)
{
    if (x != y)
    {
        return true;
    }

    return false;
}

// return -1 if x is less then 0
//         1 if x is greater then 0
//         0 if x is 0
// (test that compiler handles function calls in if-condition properly)
int get_sign(int x)
{
    int res;
    if (if_isless(x))
    {
        res = -1;
    }
    else if (if_isgreater(x))
    {
        res = 1;
    }
    else
    {
        res = x;
    }

    return res;
}
