//
// Invoke function in fact.d and check that they
// return expected values
//
public class factCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = fact.fact(0);
        System.out.print("fact.fact(0) = " + res);
        check(res, 1);

        res = fact.fact(1);
        System.out.print("fact.fact(1) = " + res);
        check(res, 1);

        res = fact.fact(2);
        System.out.print("fact.fact(2) = " + res);
        check(res, 1*2);

        res = fact.fact(3);
        System.out.print("fact.fact(3) = " + res);
        check(res, 1*2*3);

        res = fact.fact(6);
        System.out.print("fact.fact(6) = " + res);
        check(res, 1*2*3*4*5*6);

        exit();
    }
}

