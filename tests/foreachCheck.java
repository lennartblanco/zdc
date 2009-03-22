//
// Invoke function in foreach.d and check that they
// return expected values
//
public class foreachCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = foreach.foreach_slice();
        check("foreach.foreach_slice()", res, 6);

        res = foreach.foreach_slice_params(1, 2);
        check("foreach.foreach_slice_params(1, 2)", res, 11);

        res = foreach.foreach_slice_params(0, 6);
        check("foreach.foreach_slice_params(0, 6)", res,
              10 + 11 + 12 + 13 + 14 + 15);

        res = foreach.foreach_slice_params(3, 5);
        check("foreach.foreach_slice_params(3, 5)", res,
              13 + 14);



        exit();
    }
}

