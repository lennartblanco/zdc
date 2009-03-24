int foreach_slice()
{
    int[6] rofl = [0,1,2,3,4,5];
    int res = 0;
    int last_i;

    foreach (int i, int v; rofl[1..4]) {
      res = res + v;
      last_i = i;
    }

    return res + last_i;
}

int foreach_slice_params(int s, int e)
{
    int[6] rofl = [10, 11, 12,
                   13, 14, 15];
    int res = 0;

    foreach (int i, int v; rofl[s..e]) {
      res = res + v + i;
    }

    return res;
}

int foreach_whole_arry(int idx, int val)
{
    int[8] arr =
        [2, 4, 6, 8, 10, 12, 14, 16];    
    int r = 1;

    arr[idx] = val;

    foreach (int v; arr)
    {
        r = r * v;
    }
    return r;
}

