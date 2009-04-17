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


//TODO fix xdc to handle below 
//int iret_n_arg(int n, int arg1, int arg2, int arg3)
//{
//   if (n == 1) return arg1;
//   else if (n == 2) return arg2;
//   else if (n == 3) return arg3;
//   else return -1;
//}
