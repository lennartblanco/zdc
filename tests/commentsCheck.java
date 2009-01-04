/*
 * Check that comments.d seems to be parsed correctly
 */
public class commentsCheck extends check
{
    public static void main(String[] args)
    {
        int ires;
        boolean bres;

        /* check that dummy1() seems sane */
        bres = comments.dummy1(true);
        System.out.print("comments.dummy1(true) = " + bres);
        check(bres, true);

        bres = comments.dummy1(false);
        System.out.print("comments.dummy1(false) = " + bres);
        check(bres, false);

        /* check that dummy2() seems sane */
        ires = comments.dummy2();
        System.out.print("comments.dummy2() = " + ires);
        check(ires, 3);

        exit();
    }
}

