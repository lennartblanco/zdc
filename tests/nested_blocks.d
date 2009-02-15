int
nested_blocks1()
{
    int a;
    {
        int b;
        int c;
        b = 10;
        c = 20;
        a = b + c;    
    }

    return a;
}

int
nested_blocks2(int i)
{
    int outside1;
    {
        int sub_block;
        sub_block = nested_blocks1();
        outside1 = sub_block + 1;
    }

    int outside2;
    {
        int sub_block1;
        int sub_block2;
        sub_block1 = 3;
        sub_block2 = 42;
        {
            int sub_sub_block;
            sub_sub_block = nested_blocks1();
            outside2 = sub_block1 * sub_block2 + sub_sub_block;
        }
    }

    return (outside1 - outside2) * i;
}

int
nested_blocks_with_if(bool g)
{
    int a;
    {
        if (g)
        {
            int b;
            int c;
            b = 10;
            c = 20;
            a = b + c;
        }
        else
        {
            a = 1123;
        }
    }

    return a;
}

int
nested_blocks_with_if2(int arg)
{
    int res;
    int zero = 0;
    {
        int i;
        i = zero;
        if (arg < 0)
        {
            int minus_one = -1;
            i = minus_one;
        }
        else if (arg > 0)
        {
            int one = 1;
            i = one;
        }
        res = i;
    }
    return res;
}

