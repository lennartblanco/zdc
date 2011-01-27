// test parsing of comments

//C++ style

//
// C++ style,
// but multiline!
//

/* C style, on a single line */

/* C style with a * in the middle */

bool dummy1(bool a)
{
    return a;
}
/*
 Now, just C style,
 but now it's multi-line !
 */

/*
 * Now, just C style,
 * but now it's multi-line !
 */

/*************************
 * C style stars comment *
 *************************/
int
dummy2() { return 3; }
/****
 * foo
 */

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  bool
  call_dummy1(bool arg)
  {
    return dummy1(arg);
  }

  int
  call_dummy2()
  {
    return dummy2();
  }
}

