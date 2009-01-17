/*
 * Invoke function in stat_array.d and check that they
 * return expected values
 */
public class stat_arrayCheck extends check
{
    public static void main(String[] args)
    {
        int ires;
        boolean bres;

        /* intops() tests */
        ires = stat_array.intops(3, 1);
        check("stat_array.intops(3, 1)", ires, 3 + 20 + 30 + 20);

        ires = stat_array.intops(-12, 2);
        check("stat_array.intops(3, 1)", ires, -12 + 20 + 30 + 30);

        /* boolops() tests */
        bres = stat_array.boolops(true, true, false, 2);
        check("stat_array.boolops(true, true, false, 2)", bres, true);

        bres = stat_array.boolops(false, true, false, 1);
        check("stat_array.boolops(false, true, false, 1)", bres, false);

        /* boolops2() tests */
        bres = stat_array.boolops2(true, true, false, 2);
        check("stat_array.boolops2(true, true, false, 2)", bres, true);

        bres = stat_array.boolops2(false, true, false, 1);
        check("stat_array.boolops2(false, true, false, 1)", bres, false);

        /* call_sum_stat() and sum_stat() tests */
        ires = stat_array.call_sum_stat();
        check("stat_array.call_sum_stat()", ires, 13);

        /* slice_assig_sum() and slices_ops() tests */
        ires = stat_array.slices_ops(85);
        check("stat_array.slices_ops(85)", ires, -85);

        /* init_exp_tst() test */
        check("stat_array.init_exp_tst(2)",
               stat_array.init_exp_tst(2), -22);

        exit();
    }
}

