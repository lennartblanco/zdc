void no_args()
{
   int length;
   int width;
   int area;

   length = 2;
   width = 10;

   area = length * width * 2;

   one_arg(10);
}

void one_arg(int x)
{
    x = 10;
}

void two_args(int one, int two)
{
    int three;

    three = one + two; 
}

int
three_args(int one, int two, int three)
{
    return one + two + three;
}

void no_body()
{
    int foo;
}


