int
while_tst1(int x)
{
    int res = 0;
    while (res < 20)
    {
        res = res + x;
    }

    return res;
}

int
while_tst2(int x)
{
    int res = 0;
    while (res < 20)
        res = res + x;

    return res;
}
