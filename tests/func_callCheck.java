//
// This code can be used, after some message, to
// check the java accembly generated for func_call.d
//
public class func_callCheck
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

        res = foo.add(-10,1);
        System.out.print("foo.add(-10, 1) = " + res);
        check(res, -9);

        res = foo.get_13();
        System.out.print("foo.get_13() = " + res);
        check(res, 13);

        res = foo.foo();
        System.out.print("foo.foo() = " + res);
        check(res, 14);

        res = foo.hej();
        System.out.print("foo.hej() = " + res);
        check(res, 24);
    }
}

