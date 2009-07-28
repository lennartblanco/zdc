public class bool_opCheck extends check
{
    public static void main(String[] args)
    {
        boolean res;

        res = bool_op.return_true();
        check("bool_op.return_true()", res, true);

        res = bool_op.return_false();
        check("bool_op.return_false()", res, false);

        res = bool_op.is_less(1, 2);
        check("bool_op.is_less(1, 2)", res, true);

        res = bool_op.is_less(100, 8);
        check("bool_op.is_less(100, 8)", res, false);

        res = bool_op.less_tst1();
        check("bool_op.less_tst1()", res, true);

        res = bool_op.less_tst2(2);
        check("bool_op.less_tst2(2)", res, true);

        res = bool_op.less_tst2(-1);
        check("bool_op.less_tst2(-1)", res, false);

        res = bool_op.less_tst3(2);
        check("bool_op.less_tst3(2)", res, true);

        res = bool_op.less_tst3(-1);
        check("bool_op.less_tst3(-1)", res, false);

        res = bool_op.is_greater(1, 2);
        check("bool_op.is_greater(1, 2)", res, false);

        res = bool_op.is_greater(100, 8);
        check("bool_op.is_greater(100, 8)", res, true);

        res = bool_op.greater_tst1();
        check("bool_op.greater_tst1()", res, false);

        res = bool_op.greater_tst2(0);
        check("bool_op.greater_tst2(0)", res, true);

        res = bool_op.greater_tst2(-1);
        check("bool_op.greater_tst2(-1)", res, true);

        res = bool_op.greater_tst3(2);
        check("bool_op.greater_tst3(2)", res, false);

        res = bool_op.greater_tst3(-1);
        check("bool_op.greater_tst3(-1)", res, true);

        res = bool_op.is_less_or_equal(1, 2);
        check("bool_op.is_less_or_equal(1, 2)", res, true);

        res = bool_op.is_less_or_equal(2, 2);
        check("bool_op.is_less_or_equal(2, 2)", res, true);

        res = bool_op.is_less_or_equal(1, -2);
        check("bool_op.is_less_or_equal(1, -2)", res, false);

        res = bool_op.is_greater_or_equal(1, 2);
        check("bool_op.is_greater_or_equal(1, 2)", res, false);

        res = bool_op.is_greater_or_equal(2, 2);
        check("bool_op.is_greater_or_equal(2, 2)", res, true);

        res = bool_op.is_greater_or_equal(1, -2);
        check("bool_op.is_greater_or_equal(1, -2)", res, true);

        res = bool_op.is_equal(20, 45);
        check("bool_op.is_equal(20, 45)", res, false);

        res = bool_op.is_equal(983, 983);
        check("bool_op.is_equal(983, 983)", res, true);

        res = bool_op.not_equal(57, -35);
        check("bool_op.not_equal(57, -35)", res, true);

        res = bool_op.not_equal(0, 0);
        check("bool_op.not_equal(0, 0)", res, false);

        /* tests on and_oper() */

        res = bool_op.and_oper(false, false);
        check("bool_op.and_oper(false, false)", res, false);

        res = bool_op.and_oper(true, false);
        check("bool_op.and_oper(true, false)", res, false);

        res = bool_op.and_oper(false, true);
        check("bool_op.and_oper(false, true)", res, false);

        res = bool_op.and_oper(true, true);
        check("bool_op.and_oper(true, true)", res, true);

        /* or_oper() tests */

        res = bool_op.or_oper(false, false);
        check("bool_op.or_oper(false, false)", res, false);

        res = bool_op.or_oper(true, false);
        check("bool_op.or_oper(true, false)", res, true);

        res = bool_op.or_oper(false, true);
        check("bool_op.or_oper(false, true)", res, true);

        res = bool_op.or_oper(true, true);
        check("bool_op.or_oper(true, true)", res, true);

        /* tripple_and_op() tests */
        res = bool_op.tripple_and_op(false, false, false);
        check("bool_op.tripple_and_op(false, false, false)", res, false);

        /* tripple_and_op() tests */
        res = bool_op.tripple_and_op(false, true, false);
        check("bool_op.tripple_and_op(false, true, false)", res, false);

        /* tripple_and_op() tests */
        res = bool_op.tripple_and_op(true, true, true);
        check("bool_op.tripple_and_op(true, true, true)", res, true);

        /* andor_ops() tests */
        res = bool_op.andor_ops(true, true, true);
        check("bool_op.andor_ops(true, true, true)", res, true);

        res = bool_op.andor_ops(false, true, true);
        check("bool_op.andor_ops(false, true, true)", res, true);

        res = bool_op.andor_ops(false, false, true);
        check("bool_op.andor_ops(false, false, true)", res, true);

        res = bool_op.andor_ops(true, true, false);
        check("bool_op.andor_ops(true, true, false)", res, true);

        res = bool_op.andor_ops(false, false, false);
        check("bool_op.andor_ops(false, false, false)", res, false);

        exit();
    }
}
