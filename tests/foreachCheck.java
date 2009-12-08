//
// Invoke function in foreach.d and check that they
// return expected values
//
public class foreachCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        /* foreach_slice() tests */
        res = foreach.foreach_slice();
        check("foreach.foreach_slice()", res,
              (1 + 2 + 3) + 2);

        /* foreach_slice_params() tests */
        res = foreach.foreach_slice_params(1, 2);
        check("foreach.foreach_slice_params(1, 2)", res, 11);

        res = foreach.foreach_slice_params(0, 6);
        check("foreach.foreach_slice_params(0, 6)", res,
              (10 + 11 + 12 + 13 + 14 + 15) +
              (0 + 1 + 2 + 3 + 4 + 5));

        res = foreach.foreach_slice_params(3, 5);
        check("foreach.foreach_slice_params(3, 5)", res,
              (13 + 14) + (0 + 1));

        /* foreach_whole_arry() tests */
        res = foreach.foreach_shorthand_slice(0, -1);
        check("foreach_shorthand_slice(0, -1)", res,
              -1 * 4 * 6 * 8 * 10 * 12 * 14 * 16);

        res = foreach.foreach_shorthand_slice(4, 3);
        check("foreach_shorthand_slice(4, 3)", res,
              2 * 4 * 6 * 8 * 3 * 12 * 14 * 16);


        exit();
    }
}

