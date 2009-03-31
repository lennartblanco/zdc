public class retsCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        rets.void_return();
        check("rets.void_return()");

        res = rets.int_ret();
        check("rets.int_ret()", res, 10);

        res = rets.iret_val(-2);
        check("rets.iret_val(-2)", res, -2);
    }
}
