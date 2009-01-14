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
        check("fact.fact(0)", res, 1);

        res = fact.fact(1);
        check("fact.fact(1)", res, 1);

        res = fact.fact(2);
        check("fact.fact(2)", res, 1*2);

        res = fact.fact(3);
        check("fact.fact(3)", res, 1*2*3);

        res = fact.fact(6);
        check("fact.fact(6)", res, 1*2*3*4*5*6);

        exit();
    }
}

