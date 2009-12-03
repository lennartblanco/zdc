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

int foreach_shorthand_slice(int idx, int val)
{
    int[8] arr =
        [2, 4, 6, 8, 10, 12, 14, 16];    
    int r = 1;

    arr[idx] = val;

    foreach (int v; arr[])
    {
        r = r * v;
    }
    return r;
}

bool foreach_value_auto_type(int l)
{
   bool res = false;
   int[5] arr = [3, 4, 15, -3, 4];

   foreach (v; arr[])
   {
       res = res || v > l;
   }

   return res;
}

//
// test cases
//  int[6] arry = [97,3,211,245,128,36];
// foreach_index_auto_type(97, arry)    = 0
// foreach_index_auto_type(211, arry[]) = 2
// foreach_index_auto_type(36, arry[])  = 5
// foreach_index_auto_type(-15, arry[]) = -1
// foreach_index_auto_type(4, arry[]) = -1

int foreach_index_auto_type(int needle, int[6] haystack)
{
   int pos = -1;

   foreach (i, int v; haystack[])
   {
       if (v == needle)
       {
           pos = i;
       }
   }
   return pos;
}

