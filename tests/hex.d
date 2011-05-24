enum BIT_MASK = 0x00000F0F;

extern (C) int
hex_int(uint idx)
{
  int i0 = 0x0;
  int i1 = 0X00fa;
  int i2;

  i2 = 0x7FFFFFFF;

  if (0 == idx)
  {
    return i0;
  }
  else if (1 == idx)
  {
    return i1;
  }
  else if (2 == idx)
  {
    return i2;
  }
  else if (3 == idx)
  {
    return 0x00004000;
  }
  else if (4 == idx)
  {
    return i1 - 0x000000f0;
  }
  else if (5 == idx)
  {
    return BIT_MASK;
  }
  
  return -0x01;;
}

enum BIT_MASK_U = 0x00000FAFu;

extern (C) uint
hex_uint(uint idx)
{
  uint u0 = 0xA3u;
  uint u1;
  u1 = 0X1FFU;
  uint u2 = 0x80000000;

  if (0 == idx)
  {
    return u0;
  }
  else if (1 == idx)
  {
    return u1;
  }
  else if (2 == idx)
  {
    return u2 + 0x1u;
  }
  else if (3 == idx)
  {
    return 0X80001230;
  }
  else if (4 == idx)
  {
    return BIT_MASK_U;
  }

  return 0x0U;
}
