//
// Invoke function in fact.d and check that they
// return expected values
//
public class while_loopCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = while_loop.while_tst1(10);
        check("while_loop.while_tst1(10)", res, 20);

        res = while_loop.while_tst1(3);
        check("while_loop.while_tst1(3)", res, 21);

        res = while_loop.while_tst1(19);
        check("while_loop.while_tst1(19)", res, 19*2);

        res = while_loop.while_tst1(123);
        check("while_loop.while_tst1(123)", res, 123);

        exit();
    }
}

