//
// Test that uninitialized variables are correctly set to 
// default initializer value
//

bool uninit_bool()
{
    bool x;

    return x;
}

char uninit_char()
{
    char x;

    return x;
}

int uninit_int()
{
    int x;

    return x;
}

uint uninit_uint()
{
    uint x;

    return x;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
    bool
    call_uninit_bool()
    {
      return uninit_bool();
    }

    char
    call_uninit_char()
    {
      return uninit_char();
    }

    int
    call_uninit_int()
    {
      return uninit_int();
    }

    uint
    call_uninit_uint()
    {
      return uninit_uint();
    }
}
