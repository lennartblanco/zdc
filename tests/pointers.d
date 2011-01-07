extern (C) void *malloc(uint);

int *
def_int_ptr()
{
    int *ptr;

    return ptr;
}

char *
def_char_ptr()
{
    char *ptr;
    
    return ptr;
}

// will allocate 8 bytes, which must be free():ed
void *
malloced_ptr()
{
    return malloc(8);
}

