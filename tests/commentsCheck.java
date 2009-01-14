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
        check("comments.dummy1(true)", bres, true);

        bres = comments.dummy1(false);
        check("comments.dummy1(false)", bres, false);

        /* check that dummy2() seems sane */
        ires = comments.dummy2();
        check("comments.dummy2()", ires, 3);

        exit();
    }
}

