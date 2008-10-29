int
fact(int i)
{
    if (i <= 0)
    {
        return 1;
    }

    return i * fact(i-1);
}

