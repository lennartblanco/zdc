void
void_return()
{
    return;
}

int
int_ret()
{
    return 10;
}

int
iret_val(int val) { return val; }

/* return value of first argument */
int iret_arg1(int arg1, int arg2, int arg3)
{
   return arg1;
}

/* return value of first argument via a local variable */
int iret_arg1_with_ret_var(int arg1, int arg2, int arg3)
{
   int ret = arg1;
   return ret;
}

int iret_arg2(int arg1, int arg2, int arg3)
{
   return arg2;
}

int iret_arg3(int arg1, int arg2, int arg3)
{
   return arg3;
}

/* void function, with empty body (that is an implicit return) */
void printNum(int foo)
{

}

/* void function with an implicit return */
void no_ret_void(int a, int b)
{
    int x;

    x = a + b;
    printNum(x);
}

/* void function without any arguments and empty body */
void empty_func() {}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  void
  call_void_return()
  {
     void_return();
  }

  void
  call_printNum(int arg)
  {
    printNum(arg);

  }

  int
  call_int_ret()
  {
    return int_ret();
  }

  int
  call_iret_val(int arg)
  {
    return iret_val(arg);
  }

  int
  call_iret_arg1(int arg1, int arg2, int arg3)
  {
    return iret_arg1(arg1, arg2, arg3);
  }

  int
  call_iret_arg1_with_ret_var(int arg1, int arg2, int arg3)
  {
    return iret_arg1_with_ret_var(arg1, arg2, arg3);
  }

  int
  call_iret_arg2(int arg1, int arg2, int arg3)
  {
    return iret_arg2(arg1, arg2, arg3);
  }

  int
  call_iret_arg3(int arg1, int arg2, int arg3)
  {
    return iret_arg3(arg1, arg2, arg3);
  }

  void
  call_no_ret_void(int arg1, int arg2)
  {
    no_ret_void(arg1, arg2);
  }

  void
  call_empty_func()
  {
    empty_func();
  }
}

