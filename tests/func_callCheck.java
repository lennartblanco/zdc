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
        check("func_call.add(-10,1)", res, -9);

        res = func_call.get_13();
        check("func_call.get_13()", res, 13);

        res = func_call.foo();
        check("func_call.foo()", res, 14);

        res = func_call.hej();
        check("func_call.hej()", res, 24);

        res = func_call.sum(-1, 0, 1);
        check("func_call.sum(-1, 0, 1)", res, 0);

        res = func_call.call_sum(5);
        check("func_call.call_sum(5)", res, 5 + 13 + 10);

        exit();
    }
}

