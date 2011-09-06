/*
 * explicit cast between basic data types
 */

extern (C)
{
  /* casts to bool */

  bool
  bool_to_bool(bool arg)
  {
    return cast(bool)arg;
  }

  /* casts to char */

  char
  bool_to_char(bool arg)
  {
    return cast(char)arg;
  }

  char
  char_to_char(char arg)
  {
    return cast(char)arg;
  }

  char
  byte_to_char(byte arg)
  {
    char r = cast(char)arg;
    return r;
  }

  char
  ubyte_to_char(ubyte arg)
  {
    return cast(char)arg;
  }

  /* casts to byte */

  byte
  bool_to_byte(bool arg)
  {
    byte r = cast(byte)arg;

    return r;
  }

  byte
  char_to_byte(char arg)
  {
    return cast(byte)arg;
  }

  byte
  byte_to_byte(byte arg)
  {
    return cast(byte)arg;
  }

  byte
  ubyte_to_byte(ubyte arg)
  {
    return cast(byte)arg;
  }

  /* casts to ubyte */

  ubyte
  cast_to_ubyte(uint testno)
  {
    /* bool to ubyte */
    if (0 == testno)
    {
      bool a;
      return cast(ubyte)a;
    }
    else if (1 == testno)
    {
      bool a = true;
      return cast(ubyte)a;
    }

    /* byte to ubyte */
    else if (2 == testno)
    {
      byte b = 123;
      return cast(ubyte)b;
    }
    else if (3 == testno)
    {
      byte b = -8;
      return cast(ubyte)b;
    }   

    /* char to ubyte */
    else if (4 == testno)
    {
      char b = 'a';
      return cast(ubyte)b;
    }
    else if (5 == testno)
    {
      char b = '\0';
      return cast(ubyte)b;
    }   

    /* ubyte to ubyte */
    else if (6 == testno)
    {
      ubyte b;
      return cast(ubyte)b;
    }
    else if (7 == testno)
    {
      ubyte b = 42;
      return cast(ubyte)b;
    }   

    return 255;
  }

  /* casts to short */
  short
  cast_to_short(uint testno)
  {
    /* short to short */
    if (0 == testno)
    {
      short s = -10;
      return cast(short)s;
    }
    else if (1 == testno)
    {
      short s = 10;
      return cast(short)s;
    }

    /* ushort to short */
    else if (2 == testno)
    {
      ushort s = 100;
      return cast(short)s;
    }
    else if (3 == testno)
    {
      ushort s = 65534;
      return cast(short)s;
    }

    return -1;
  }

  /* cast to ushort */
  ushort
  cast_to_ushort(uint testno)
  {
    /* short to ushort */
    if (0 == testno)
    {
      short s = -4;
      return cast(ushort)s;
    }
    else if (1 == testno)
    {
      short s = 123;
      return cast(ushort)s;
    }
    /* ushort to ushort */
    else if (2 == testno)
    {
      ushort u;
      return cast(ushort)u;
    }
    else if (3 == testno)
    {
      ushort u = 257;
      return cast(ushort)u;
    }
    return 65535;
  }

  /* cast to int */
  int
  cast_to_int(uint testno)
  {
    /* int to int */
    if (0 == testno)
    {
      int i = 17002;
      return cast(int)i;
    }
    else if (1 == testno)
    {
      int i = -302;
      return cast(int)i;
    }

    /* uint to int */
    else if (2 == testno)
    {
      uint u = 7;
      return cast(int)u;
    }
    else if (3 == testno)
    {
      uint u = 4294967294;
      return cast(int)u;
    }
    return -1;
  }

  /* cast to uint */
  uint
  cast_to_uint(uint testno)
  {
    /* int to uint */
    if (0 == testno)
    {
      int x = 32000;
      return cast(uint)x;
    }
    else if (1 == testno)
    {
      int x = -3;
      return cast(uint)x;
    }

    /* uint to uint */
    else if (2 == testno)
    {
      uint u = 3;
      return cast(uint)u;
    }
    else if (3 == testno)
    {
      uint u = 2147483650;
      return cast(uint)u;
    }

    return 0;
  }
}

