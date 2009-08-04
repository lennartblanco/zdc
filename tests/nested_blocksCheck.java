//
// Invoke function in nested_blocks.d and check that they
// return expected values
//
public class nested_blocksCheck extends check
{
    public static void main(String[] args)
    {
        int res;

        res = nested_blocks.nested_blocks1();
        check("nested_blocks.nested_blocks1()", res, 30);

        res = nested_blocks.nested_blocks2(8);
        check("nested_blocks.nested_blocks2()", res, -1000);

        /* test nested_blocks_with_if() */
        res = nested_blocks.nested_blocks_with_if(true);
        check("nested_blocks.nested_blocks_with_if(true)", res, 30);

        res = nested_blocks.nested_blocks_with_if(false);
        check("nested_blocks.nested_blocks_with_if(false)", res, 1123);

        /* test nested_blocks_with_if2() */
        res = nested_blocks.nested_blocks_with_if2(-20);
        check("nested_blocks.nested_blocks_with_if2(-20)", res, -1);

        res = nested_blocks.nested_blocks_with_if2(36);
        check("nested_blocks.nested_blocks_with_if2(36)", res, 1);

        res = nested_blocks.nested_blocks_with_if2(0);
        check("nested_blocks.nested_blocks_with_if2(0)", res, 0);

        exit();
    }
}

