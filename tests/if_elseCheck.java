//
// Invoke function in if_else.d and check that they
// return expected values
//
public class if_elseCheck extends check
{
    public static void main(String[] args)
    {
        boolean res;
        int ires;

        /* if_else_is_less() tests */
        res = if_else.if_isless(-1);
        System.out.print("if_else.if_else_isless(-1) = " + res);
        check(res, true);

        res = if_else.if_isless(0);
        System.out.print("if_else.if_else_isless(0) = " + res);
        check(res, false);

        res = if_else.if_isless(10);
        System.out.print("if_else.if_else_isless(10) = " + res);
        check(res, false);

        /* if_else_isgreater() tests */        
        res = if_else.if_isgreater(3);
        System.out.print("if_else.if_else_isgreater(3) = " + res);
        check(res, true);

        res = if_else.if_isgreater(-3);
        System.out.print("if_else.if_else_isgreater(-3) = " + res);
        check(res, false);

        res = if_else.if_isgreater(0);
        System.out.print("if_else.if_else_isgreater(0) = " + res);
        check(res, false);

        /* if_else_isless_eq() tests */        
        res = if_else.if_isless_eq(3);
        System.out.print("if_else.if_else_isgreater(3) = " + res);
        check(res, false);

        res = if_else.if_isless_eq(-3);
        System.out.print("if_else.if_else_isgreater(-3) = " + res);
        check(res, true);

        res = if_else.if_isless_eq(0);
        System.out.print("if_else.if_else_isgreater(0) = " + res);
        check(res, true);

        /* if_else_isgreater_eq() tests */        
        res = if_else.if_isgreater_eq(3);
        System.out.print("if_else.if_else_isgreater(3) = " + res);
        check(res, true);

        res = if_else.if_isgreater_eq(-3);
        System.out.print("if_else.if_else_isgreater(-3) = " + res);
        check(res, false);

        res = if_else.if_isgreater_eq(0);
        System.out.print("if_else.if_else_isgreater(0) = " + res);
        check(res, true);

        /* if_else_iseq() tests */        
        res = if_else.if_iseq(11, 64);
        System.out.print("if_else.if_else_iseq(11, 64) = " + res);
        check(res, false);

        res = if_else.if_iseq(998, 998);
        System.out.print("if_else.if_else_iseq(998, 998) = " + res);
        check(res, true);

        /* if_else_isnoteq() tests */        
        res = if_else.if_isnoteq(11, 64);
        System.out.print("if_else.if_else_iseq(11, 64) = " + res);
        check(res, true);

        res = if_else.if_isnoteq(998, 998);
        System.out.print("if_else.if_else_iseq(998, 998) = " + res);
        check(res, false);

        /* abs() tests */
        ires = if_else.abs(-19);
        System.out.print("if_else.abs(-19) = " + ires);
        check(ires, 19);

        ires = if_else.abs(641);
        System.out.print("if_else.abs(641) = " + ires);
        check(ires, 641);

        ires = if_else.abs(0);
        System.out.print("if_else.abs(0) = " + ires);
        check(ires, 0);

        /* if_else_test() tests */
        ires = if_else.if_else_test(3);
        System.out.print("if_else.if_else_test(3) = " + ires);
        check(ires, 1);

        ires = if_else.if_else_test(10);
        System.out.print("if_else.if_else_test(10) = " + ires);
        check(ires, 2);

        ires = if_else.if_else_test(14);
        System.out.print("if_else.if_else_test(14) = " + ires);
        check(ires, 4);

        ires = if_else.if_else_test(15);
        System.out.print("if_else.if_else_test(14) = " + ires);
        check(ires, 3);

        exit();
    }
}

