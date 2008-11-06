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

        exit();
    }
}
