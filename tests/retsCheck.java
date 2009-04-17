public class retsCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        /* void_return() test */
        rets.void_return();
        check("rets.void_return()");

        /* int_ret() test */
        res = rets.int_ret();
        check("rets.int_ret()", res, 10);

        /* iret_val() tests */
        res = rets.iret_val(-2);
        check("rets.iret_val(-2)", res, -2);

        res = rets.iret_val(1984);
        check("rets.iret_val(1984)", res, 1984);

        /* iret_arg1() tests */
        res = rets.iret_arg1(11, 21, 31);
        check("rets.iret_arg1(11, 21, 31)", res, 11);

        res = rets.iret_arg1(9876, 1234, 789);
        check("rets.iret_arg1(9876, 1234, 789)", res, 9876);


        /* iret_arg1_with_ret_var() tests */
        res = rets.iret_arg1_with_ret_var(11, 21, 31);
        check("rets.iret_arg1_with_ret_var(11, 21, 31)", res, 11);

        res = rets.iret_arg1_with_ret_var(9876, 1234, 789);
        check("rets.iret_arg1_with_ret_var(9876, 1234, 789)", res, 9876);

        /* iret_arg2() tests */
        res = rets.iret_arg2(11, 21, 31);
        check("rets.iret_arg2(11, 21, 31)", res, 21);

        res = rets.iret_arg2(0, 1234, 789);
        check("rets.iret_arg2(0, 1234, 789)", res, 1234);

        /* iret_arg3() tests */
        res = rets.iret_arg3(11, 21, 31);
        check("rets.iret_arg3(11, 21, 31)", res, 31);

        res = rets.iret_arg3(0, 1234, 789);
        check("rets.iret_arg3(0, 1234, 789)", res, 789);


        exit();
    }
}
