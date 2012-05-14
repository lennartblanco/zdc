class empty { } /* class without any members/methods */

class orange
{
  uint size = 87;
  bool eaten;
}

extern (C):

orange
def_init_orange()
{
  orange or;
  return or;
}

/**
 * create new object and test setting and getting members
 */
uint
orange_set_get(uint inc, bool eaten)
{
  orange o = new orange;

  o.eaten = eaten;
  o.size = o.size + inc;

  return o.eaten ? o.size : 100-o.size;
}
