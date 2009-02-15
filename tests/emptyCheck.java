//
// Check that we can 'link' againsted compiled empty.d file
//
public class emptyCheck extends check
{
    public static void main(String[] args)
    {
        empty e = new empty();
        check("new emtpty()", e != null, true);

        exit();
    }
}

