int abs(int a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
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

