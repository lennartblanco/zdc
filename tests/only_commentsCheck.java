//
// Check that we can 'link' againsted compiled only_comments.d file
//
public class only_commentsCheck extends check
{
    public static void main(String[] args)
    {
        only_comments o = new only_comments();
        check("new only_comments()", o != null, true);

        exit();
    }
}

