//
// test calling C functions from D file
//

//
// external C function declaration in varius styles
//
extern (C) int cinv(int c);
extern (C) bool c_is_equal(int, int);

extern (C)
{
   int cadd(int left, int right);
   int csubst3(int a, int b, int c);
   int cexpr(int a, int b, int c, int d);
}

extern (D)
{
  extern (C) int cget20();
  extern (C)
  {

      void c_store_num(int num);
      int c_get_num();
  }
}

//
// D wrapper functions to call external C functions
//

// cinv() wrapper
int
cinv_wrap(int num)
{
    return cinv(num);
}

// cadd() wrapper
int
cadd_wrap(int l, int r)
{
    return cadd(l, r);
}

// csubst3() wrapper
int
csubst3_wrap(int arg1, int arg2, int arg3)
{
    int res;

    res = csubst3(arg1, arg2, arg3);

    return res;
}

// cexpr() wrapper
int
cexpr_wrap(int arg1, int arg2, int arg3, int arg4)
{
    int res;

    res = cexpr(arg1, arg2, arg3, arg4);

    return res;
}

// cget20() wrapper
int
cget20_wrap()
{
    return cget20();
}

// c_store_num() wrapper
void
c_store_num_wrap(int x)
{
    c_store_num(x);
}

// c_get_num() wrapper
int
c_get_num_wrap()
{
    return c_get_num();
}

// c_is_equal() wrapper
bool
c_is_equal_wrap(int left, int right)
{
    return c_is_equal(left, right);
}


