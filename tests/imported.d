struct lemon
{
  uint age;
  int bitterness;
}

alias uint orange;

extern (C) void ext_init_lemon(lemon *);

int bit_per_years(lemon *l)
{
  return l.bitterness / l.age;
}
