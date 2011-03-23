void
mset32(int source, uint nums, int *dest)
{
    int *last = dest + nums;

    while (dest < last)
    {
      *dest = source;
      dest = dest + 1;
    }
}

