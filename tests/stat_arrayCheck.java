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
        System.out.print("stat_array.intops(3, 1) = " + ires);
        check(ires, 3 + 20 + 30 + 20);

        ires = stat_array.intops(-12, 2);
        System.out.print("stat_array.intops(3, 1) = " + ires);
        check(ires, -12 + 20 + 30 + 30);

        /* boolops() tests */
        bres = stat_array.boolops(true, true, false, 2);
        System.out.print("stat_array.boolops(true, true, false, 2) = " + bres);
        check(bres, true);

        bres = stat_array.boolops(false, true, false, 1);
        System.out.print("stat_array.boolops(false, true, false, 1) = " + bres);
        check(bres, false);

        /* call_sum_stat() and sum_stat() tests */
        ires = stat_array.call_sum_stat();
        System.out.print("stat_array.call_sum_stat() = " + ires);
        check(ires, 7);

        /* slice_assig_sum() and slices_ops() tests */
        ires = stat_array.slices_ops(85);
        System.out.print("stat_array.slices_ops(85) = " + ires);
        check(ires, -85);

        exit();
    }
}

