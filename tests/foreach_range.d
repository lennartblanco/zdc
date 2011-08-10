extern (C)
{
  int foreach_range(int lower, int upper)
  {
    int res;

    foreach (i; lower..upper)
    {
      res = res + i;
    }

    return res;
  }

  int foreach_range_break(int init, int max)
  {
    int res = init;

    foreach (x ; 0 .. 10)
    {
      if (res >= max)
      {
        break;
      }
      res++;
    }

    return res;
  }
}
