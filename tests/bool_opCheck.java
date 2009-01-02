public class bool_opCheck extends check
{
    public static void main(String[] args)
    {
        boolean res;

        res = bool_op.return_true();
        System.out.print("bool_op.return_true() = " + res);
        check(res, true);

        res = bool_op.return_false();
        System.out.print("bool_op.return_false() = " + res);
        check(res, false);

        res = bool_op.is_less(1, 2);
        System.out.print("bool_op.is_less(1, 2) = " + res);
        check(res, true);

        res = bool_op.is_less(100, 8);
        System.out.print("bool_op.is_less(100, 8) = " + res);
        check(res, false);

        res = bool_op.less_tst1();
        System.out.print("bool_op.less_tst1() = " + res);
        check(res, true);

        res = bool_op.less_tst2(2);
        System.out.print("bool_op.less_tst2(2) = " + res);
        check(res, true);

        res = bool_op.less_tst2(-1);
        System.out.print("bool_op.less_tst3(-1) = " + res);
        check(res, false);

        res = bool_op.less_tst3(2);
        System.out.print("bool_op.less_tst3(2) = " + res);
        check(res, true);

        res = bool_op.less_tst3(-1);
        System.out.print("bool_op.less_tst3(-1) = " + res);
        check(res, false);

        res = bool_op.is_greater(1, 2);
        System.out.print("bool_op.is_greater(1, 2) = " + res);
        check(res, false);

        res = bool_op.is_greater(100, 8);
        System.out.print("bool_op.is_greater(100, 8) = " + res);
        check(res, true);

        res = bool_op.greater_tst1();
        System.out.print("bool_op.greater_tst1() = " + res);
        check(res, false);

        res = bool_op.greater_tst2(0);
        System.out.print("bool_op.greater_tst2(0) = " + res);
        check(res, true);

        res = bool_op.greater_tst2(-1);
        System.out.print("bool_op.greater_tst2(-1) = " + res);
        check(res, true);

        res = bool_op.greater_tst3(2);
        System.out.print("bool_op.greater_tst3(2) = " + res);
        check(res, false);

        res = bool_op.greater_tst3(-1);
        System.out.print("bool_op.greater_tst3(-1) = " + res);
        check(res, true);

        res = bool_op.is_less_or_equal(1, 2);
        System.out.print("bool_op.is_less_or_equal(1, 2) = " + res);
        check(res, true);

        res = bool_op.is_less_or_equal(2, 2);
        System.out.print("bool_op.is_less_or_equal(2, 2) = " + res);
        check(res, true);

        res = bool_op.is_less_or_equal(1, -2);
        System.out.print("bool_op.is_less_or_equal(1, -2) = " + res);
        check(res, false);

        res = bool_op.is_greater_or_equal(1, 2);
        System.out.print("bool_op.is_greater_or_equal(1, 2) = " + res);
        check(res, false);

        res = bool_op.is_greater_or_equal(2, 2);
        System.out.print("bool_op.is_greater_or_equal(2, 2) = " + res);
        check(res, true);

        res = bool_op.is_greater_or_equal(1, -2);
        System.out.print("bool_op.is_greater_or_equal(1, -2) = " + res);
        check(res, true);

        res = bool_op.is_equal(20, 45);
        System.out.print("bool_op.is_equal(20, 45) = " + res);
        check(res, false);

        res = bool_op.is_equal(983, 983);
        System.out.print("bool_op.is_equal(983, 983) = " + res);
        check(res, true);

        res = bool_op.not_equal(57, -35);
        System.out.print("bool_op.not_equal(57, -35) = " + res);
        check(res, true);

        res = bool_op.not_equal(0, 0);
        System.out.print("bool_op.not_equal(0, 0) = " + res);
        check(res, false);

        /* tests on and_oper() */

        res = bool_op.and_oper(false, false);
        System.out.print("bool_op.and_oper(false, false) = " + res);
        check(res, false);

        res = bool_op.and_oper(true, false);
        System.out.print("bool_op.and_oper(true, false) = " + res);
        check(res, false);

        res = bool_op.and_oper(false, true);
        System.out.print("bool_op.and_oper(false, true) = " + res);
        check(res, false);

        res = bool_op.and_oper(true, true);
        System.out.print("bool_op.and_oper(true, true) = " + res);
        check(res, true);

        /* or_oper() tests */

        res = bool_op.or_oper(false, false);
        System.out.print("bool_op.or_oper(false, false) = " + res);
        check(res, false);

        res = bool_op.or_oper(true, false);
        System.out.print("bool_op.or_oper(true, false) = " + res);
        check(res, true);

        res = bool_op.or_oper(false, true);
        System.out.print("bool_op.or_oper(false, true) = " + res);
        check(res, true);

        res = bool_op.or_oper(true, true);
        System.out.print("bool_op.or_oper(true, true) = " + res);
        check(res, true);

        /* tripple_and_op() tests */
        res = bool_op.tripple_and_op(false, false, false);
        System.out.print("bool_op.tripple_and_op(false, false, false) = " + res);
        check(res, false);

        /* tripple_and_op() tests */
        res = bool_op.tripple_and_op(false, true, false);
        System.out.print("bool_op.tripple_and_op(false, true, false) = " + res);
        check(res, false);

        /* tripple_and_op() tests */
        res = bool_op.tripple_and_op(true, true, true);
        System.out.print("bool_op.tripple_and_op(true, true, true) = " + res);
        check(res, true);

        /* andor_ops() tests */
        res = bool_op.andor_ops(true, true, true);
        System.out.print("bool_op.andor_ops(true, true, true) = " + res);
        check(res, true);

        res = bool_op.andor_ops(false, true, true);
        System.out.print("bool_op.andor_ops(false, true, true) = " + res);
        check(res, true);

        res = bool_op.andor_ops(false, false, true);
        System.out.print("bool_op.andor_ops(false, false, true) = " + res);
        check(res, true);

        res = bool_op.andor_ops(true, true, false);
        System.out.print("bool_op.andor_ops(true, true, false) = " + res);
        check(res, true);

        res = bool_op.andor_ops(false, false, false);
        System.out.print("bool_op.andor_ops(false, false, false) = " + res);
        check(res, false);

        exit();
    }
}
