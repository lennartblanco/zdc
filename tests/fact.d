// factorial function, implemented with recursion
int
fact_rec(int i)
{
    if (i <= 0)
    {
        return 1;
    }

    return i * fact_rec(i-1);
}

// factorial function, implemented with a while-loop
int
fact_while(int i)
{
    int res = 1;
    while (i > 1)
    {
        res = res * i;
        i = i - 1;
    }

    return res;
}

