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

//
// test cases
//   uint[4] idxs = [0, 0, 0, 0];
//   writefln("foreach_body_loc_vars([%s]) = %s", idxs, foreach_body_loc_vars(idxs));
//   idxs = [2, 4, 5, 3];
//   writefln("foreach_body_loc_vars([%s]) = %s", idxs, foreach_body_loc_vars(idxs));
//   idxs = [15, 14, 13, 12];
//   writefln("foreach_body_loc_vars([%s]) = %s", idxs, foreach_body_loc_vars(idxs));
//   idxs = [0, 1, 18, 3];
//   writefln("foreach_body_loc_vars([%s]) = %s", idxs, foreach_body_loc_vars(idxs));
//
int
foreach_body_loc_vars(uint[4] idxs)
{
    int sum;
    int[16] nums = 
        [19341, 06612, 48352, 49081, 23112,  28360, 41631, 62064,
         43166, 41808, 4188, 59660, 37889,  38528, 56732, 41092];

   foreach (nums_idx; idxs[])
   {
     int nums_val;

     if (nums_idx > 15) {
         return -1;
     }
     nums_val = nums[nums_idx];
     sum = sum + nums_val;
   }

   return sum;
}

//
// tests cases:
//
// bool[6] ops = [ false, false, false, false, false, false]
// foreach_bool_arry(ops[]) = 0
//
// bool[6] ops = [true, true, true, true, true, true]
// foreach_bool_arry(ops[]) = 11 + 12 + 13 + 14 + 15 + 16
//
// bool[6] ops = [ false, false, true, false, false, true]
// foreach_bool_arry(ops[]) = 13 + 16
//
// bool[6] ops = [ true, false, false, true, false, true]
// foreach_bool_arry(ops[]) = 11 + 14 + 16
//
// bool[6] ops = [true, false, true, false, false, false]
// foreach_bool_arry(ops[]) = 11 + 13

uint foreach_bool_arry(bool[6] add_op)
{
   uint res;
   uint[6] vals = [11, 12, 13, 14, 15, 16];

   foreach (i, op; add_op[])
   {
      if (op) { res = res + vals[i]; }
   }   

   return res;
}

