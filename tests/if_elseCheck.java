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
        check("if_else.if_isless(-1)", res, true);

        res = if_else.if_isless(0);
        check("if_else.if_isless(0)", res, false);

        res = if_else.if_isless(10);
        check("if_else.if_isless(10)", res, false);

        /* if_else_isgreater() tests */        
        res = if_else.if_isgreater(3);
        check("if_else.if_isgreater(3)", res, true);

        res = if_else.if_isgreater(-3);
        check("if_else.if_isgreater(-3)", res, false);

        res = if_else.if_isgreater(0);
        check("if_else.if_isgreater(0)", res, false);

        /* if_else_isless_eq() tests */        
        res = if_else.if_isless_eq(3);
        check("if_else.if_isless_eq(3)", res, false);

        res = if_else.if_isless_eq(-3);
        check("if_else.if_isless_eq(-3)", res, true);

        res = if_else.if_isless_eq(0);
        check("if_else.if_isless_eq(0)", res, true);

        /* if_else_isgreater_eq() tests */        
        res = if_else.if_isgreater_eq(3);
        check("if_else.if_isgreater_eq(3)", res, true);

        res = if_else.if_isgreater_eq(-3);
        check("if_else.if_isgreater_eq(-3)", res, false);

        res = if_else.if_isgreater_eq(0);
        check("if_else.if_isgreater_eq(0)", res, true);

        /* if_else_iseq() tests */        
        res = if_else.if_iseq(11, 64);
        check("if_else.if_iseq(11, 64)", res, false);

        res = if_else.if_iseq(998, 998);
        check("if_else.if_iseq(998, 998)", res, true);

        /* if_else_isnoteq() tests */        
        res = if_else.if_isnoteq(11, 64);
        check("if_else.if_isnoteq(11, 64)", res, true);

        res = if_else.if_isnoteq(998, 998);
        check("if_else.if_isnoteq(998, 998)", res, false);

        /* abs() tests */
        ires = if_else.abs(-19);
        check("if_else.abs(-19)", ires, 19);

        ires = if_else.abs(641);
        check("if_else.abs(641)", ires, 641);

        ires = if_else.abs(0);
        check("if_else.abs(0)", ires, 0);

        /* if_else_test() tests */
        ires = if_else.if_else_test(3);
        check("if_else.if_else_test(3)", ires, 1);

        ires = if_else.if_else_test(10);
        check("if_else.if_else_test(10)", ires, 2);

        ires = if_else.if_else_test(14);
        check("if_else.if_else_test(14)", ires, 4);

        ires = if_else.if_else_test(15);
        check("if_else.if_else_test(15)", ires, 3);

        exit();
    }
}

