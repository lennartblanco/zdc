//
// This code can be used, after some message, to
// check the java accembly generated for neg.d
//
public class negCheck
{
    public static void check(int res, int expectedRes)
    {
        if (res == expectedRes) 
        {
            System.out.println(" [ok]");
        }
        else
        {
            System.out.println(" unexpected result!");
        }
    }
    public static void main(String[] args)
    {
        int res;

        res = foo.neg1(256);
        System.out.print("foo.neg1(256) = " + res);
        check(res, -256);

        res = foo.neg2(-128);
        System.out.print("foo.neg2(-128) = " + res);
        check(res, 128);

        res = foo.neg3();
        System.out.print("foo.neg3() = " + res);
        check(res, 3);

        res = foo.neg5(6);
        System.out.print("foo.neg5() = " + res);
        check(res, 46);

        res = foo.neg6(4);
        System.out.print("foo.neg6() = " + res);
        check(res, 0);
    }
}

