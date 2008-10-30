int neg1(int u)
{
   u = - u;

   return u;
}

int neg2(int u)
{
   u = -(u);

   return u;
}

int neg3()
{
    return -(neg2(3));
}

int neg4()
{
    return -neg2(1);
}

int neg5(int l)
{
   int v;

   v = 10 * 4 + l;

   return v;
}

int neg6(int l)
{
   int v;

   v = 10 * (4 + neg1(l));

   return v;
}
