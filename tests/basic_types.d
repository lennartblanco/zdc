short
short_vars(uint i)
{
  short v1;
  short v2 = 10;
  short v3 = 23u;
  short v4 = 32762u;
  short v5 = v2;
  short v6;
  short v7 = 1;

  v7 = 20;
  v6 = v4;

  if (i == 1) { return v1; }
  else if (i == 2) { return v2; }
  else if (i == 3) { return v3; }
  else if (i == 4) { return v4; }
  else if (i == 5) { return v5; }
  else if (i == 6) { return v6; }
  else if (i == 7) { return v7; }

  return 0;
}

ushort
ushort_vars(uint i)
{
  ushort v1 = 2;
  ushort v2 = 100u;
  ushort v3 = v1;
  ushort v4;
  ushort v5;
  ushort v6;

  v4 = v2;
  v5 = 10;

  if (i == 0) { return 23; }
  else if (i == 1) { return v1; }
  else if (i == 2) { return v2; }
  else if (i == 3) { return v3; }
  else if (i == 4) { return v4; }
  else if (i == 5) { return v5; }
  else if (i == 6) { return v6; }

  return 0;
}

byte
byte_vars(uint i)
{
  byte v1;
  byte v2 = 10;
  byte v3 = 27u;
  byte v4 = -120;
  byte v5 = v2;
  byte v6;
  byte v7 = 1;

  v7 = 20;
  v6 = v4;

  if (i == 1) { return v1; }
  else if (i == 2) { return v2; }
  else if (i == 3) { return v3; }
  else if (i == 4) { return v4; }
  else if (i == 5) { return v5; }
  else if (i == 6) { return v6; }
  else if (i == 7) { return v7; }

  return 0;
}

ubyte
ubyte_vars(uint i)
{
  ubyte v1;
  ubyte v2 = 3;
  ubyte v3 = 111u;
  ubyte v4 = v2;
  ubyte v5;
  ubyte v6 = 1;

  v5 = v3;
  v6 = 20;

  if (i == 1) { return v1; }
  else if (i == 2) { return v2; }
  else if (i == 3) { return v3; }
  else if (i == 4) { return v4; }
  else if (i == 5) { return v5; }
  else if (i == 6) { return v6; }

  return 0;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  short
  call_short_vars(uint i)
  {
    return short_vars(i);
  }

  ushort
  call_ushort_vars(uint i)
  {
    return ushort_vars(i);
  }

  byte
  call_byte_vars(uint i)
  {
    return byte_vars(i);
  }

  ubyte
  call_ubyte_vars(uint i)
  {
    return ubyte_vars(i);
  }
}
