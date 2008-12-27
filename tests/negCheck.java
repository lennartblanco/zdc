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
        System.out.print("neg.neg1(256) = " + res);
        check(res, -256);

        res = neg.neg2(-128);
        System.out.print("neg.neg2(-128) = " + res);
        check(res, 128);

        res = neg.neg3();
        System.out.print("neg.neg3() = " + res);
        check(res, 3);

        res = neg.neg5(6);
        System.out.print("neg.neg5() = " + res);
        check(res, 46);

        res = neg.neg51(6);
        System.out.print("neg.neg51() = " + res);
        check(res, 46);

        res = neg.neg6(4);
        System.out.print("neg.neg6() = " + res);
        check(res, 0);

        exit();
    }
}

