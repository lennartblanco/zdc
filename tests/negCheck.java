//
// Invoke function in func_call.d and check that they
// return expected values
//
public class negCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = neg.neg1(256);
        check("neg.neg1(256)", res, -256);

        res = neg.neg2(-128);
        check("neg.neg2(-128)", res, 128);

        res = neg.neg3();
        check("neg.neg3()", res, 3);

        res = neg.neg5(6);
        check("neg.neg5(6)", res, 46);

        res = neg.neg51(6);
        check("neg.neg51(6)", res, 46);

        res = neg.neg6(4);
        check("neg.neg6(4)", res, 0);

        exit();
    }
}

