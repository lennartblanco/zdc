//
// Invoke function in fact.d and check that they
// return expected values
//
public class factCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = fact.fact_rec(0);
        check("fact.fact_rec(0)", res, 1);

        res = fact.fact_rec(1);
        check("fact.fact_rec(1)", res, 1);

        res = fact.fact_rec(2);
        check("fact.fact_rec(2)", res, 1*2);

        res = fact.fact_rec(3);
        check("fact.fact_rec(3)", res, 1*2*3);

        res = fact.fact_rec(6);
        check("fact.fact_rec(6)", res, 1*2*3*4*5*6);

        res = fact.fact_while(0);
        check("fact.fact_while(0)", res, 1);

        res = fact.fact_while(1);
        check("fact.fact_while(1)", res, 1);

        res = fact.fact_while(2);
        check("fact.fact_while(2)", res, 1*2);

        res = fact.fact_while(3);
        check("fact.fact_while(3)", res, 1*2*3);

        res = fact.fact_while(6);
        check("fact.fact_while(6)", res, 1*2*3*4*5*6);


        exit();
    }
}

