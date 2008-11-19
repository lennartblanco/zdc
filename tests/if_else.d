int abs(int a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
}

bool
if_else_isless(int a)
{
    if (a < 0)
    {
        return true;
    }

    return false;
}

bool
if_else_isgreater(int x)
{
    if (x > 0)
    {
        return true;
    }

    return false;
}

bool
if_else_isless_eq(int x)
{
    if (x <= 0)
    {
        return true;
    }

    return false;
}

bool
if_else_isgreater_eq(int x)
{
    if (x >= 0)
    {
        return true;
    }

    return false;
}

bool
if_else_iseq(int x, int y)
{
    if (x == y)
    {
        return true;
    }

    return false;
}

bool
if_else_isnoteq(int x, int y)
{
    if (x != y)
    {
        return true;
    }

    return false;
}

