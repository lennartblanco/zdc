// A comment!

int sum(int first, int second, int third)
{
    return first + second + third;
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
