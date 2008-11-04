//
// Invoke function in func_call.d and check that they
// return expected values
//
public class func_callCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = func_call.add(-10,1);
        System.out.print("func_call.add(-10, 1) = " + res);
        check(res, -9);

        res = func_call.get_13();
        System.out.print("func_call.get_13() = " + res);
        check(res, 13);

        res = func_call.foo();
        System.out.print("func_call.foo() = " + res);
        check(res, 14);

        res = func_call.hej();
        System.out.print("func_call.hej() = " + res);
        check(res, 24);

        exit();
    }
}

