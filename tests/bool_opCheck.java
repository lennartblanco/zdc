public class bool_opCheck extends check
{
    public static void main(String[] args)
    {
        boolean res;

        res = bool_op.return_true();
        System.out.print("bool_op.return_true = " + res);
        check(res, true);

        res = bool_op.return_false();
        System.out.print("bool_op.return_false = " + res);
        check(res, false);

        res = bool_op.is_less(1, 2);
        System.out.print("is_less(1, 2) = " + res);
        check(res, true);

        res = bool_op.less_tst1();
        System.out.print("less_tst1() = " + res);
        check(res, true);

        exit();
    }
}
