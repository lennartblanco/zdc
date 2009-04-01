public class check
{
    static int passed = 0;
    static int failed = 0;

    protected static void check(String funcCalled)
    {
        System.out.println(funcCalled + " [ok]");
        passed += 1;
    }
 
    protected static void check(String funcCalled, int res, int expectedRes)
    {
        System.out.print(funcCalled + " = " + res);
        if (res == expectedRes) 
        {
            System.out.println(" [ok]");
            passed += 1;
        }
        else
        {
            System.out.println(" unexpected result!");
            failed += 1;
        }
    }
    protected static void check(String funcCalled,
                                boolean res, boolean expected)
    {
        System.out.print(funcCalled + " = " + res);
        if (res == expected) 
        {
            System.out.println(" [ok]");
            passed += 1;
        }
        else
        {
            System.out.println(" unexpected result!");
            failed += 1;
        }
    }

    protected static void exit()
    {
        System.out.println("passed: " + passed +
                           " failed: " + failed);
        if (failed != 0)
        {
            System.exit(-1);
        }
        System.exit(0);
    }      
}
