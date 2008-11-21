import std.stdio;

int sum_dyn(int[] a)
{
    int s = 0;

    foreach(p; a)
    {
        s += p;
    }

    return s;
}

int sum_stat(int[3] s)
{
    return s[0] + s[1] + s[2];
}

int main()
{
    int[] l = [1, 23];
    int[3] m = [-1, 0, 1];
    int[] r = [1, 2, 3, 4, 5, 6];

    writefln("sum_dyn(%x)=%d", l, sum_dyn(l));
    writefln("sum_dyn(%x)=%d", r, sum_dyn(r));
    writefln("sum_dyn(%x)=%d", m, sum_dyn(m));
    writefln();
    writefln("sum_stat(%x)=%d", m, sum_stat(m));

    return 0;
}


