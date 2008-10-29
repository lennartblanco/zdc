// A comment!

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

//int bar()
//{
//    return -foo();
//}
