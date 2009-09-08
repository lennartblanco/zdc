// A comment!

int sum(int first, int second, int third)
{
    return first + second + third;
}

int subst3(int first, int second, int third)
{
    return first - second - third;
}

int subst4(int first, int second, int third, int fourth)
{
    return first - second - third - fourth;
}



int add(int a, int b)
{
    return a+b;
}

int
get_13()
{
    return 13;
}

int foo()
{
    get_13();
    return get_13() + 1;
}

int hej()
{
    return add(10, foo());
}

int ind_sum(int l)
{
    return sum(l, get_13(), 10);
}

// function call in variable initilization expression
int add_13(int i)
{
    int x = get_13();

    return x + i;
}
