/*
 * explicit cast between basic data types
 */

extern (C)
{
  /* casts to bool */

  bool
  cast_to_bool(uint testno)
  {
    /* bool to bool */
    if (0 == testno)
    {
      bool b = true;
      return cast(bool)b;
    }
    else if (1 == testno)
    {
      bool b = false;
      return cast(bool)b;
    }
    /* byte to bool */
    else if (2 == testno)
    {
      byte b;
      return cast(bool)b;
    }
    else if (3 == testno)
    {
      byte b = -10;
      return cast(bool)b;
    }
    /* char to bool */
    else if (4 == testno)
    {
      char c;
      return cast(bool)c;
    }
    else if (5 == testno)
    {
      char c = '\0';
      return cast(bool)c;
    }
    /* ubyte to bool */
    else if (6 == testno)
    {
      ubyte u;
      return cast(bool)u++;
    }
    else if (7 == testno)
    {
      ubyte u;
      return cast(bool)++u;
    }
    else if (8 == testno)
    {
      ubyte u = 10;
      return cast(bool)u;
    }
    /* short to bool */
    else if (9 == testno)
    {
      short s = 10;
      return cast(bool)s;
    }
    else if (10 == testno)
    {
      short s;
      return cast(bool)s;
    }
    /* ushort to bool */
    else if (11 == testno)
    {
      ushort s = 42;
      return cast(bool)s;
    }
    else if (12 == testno)
    {
      ushort s = 0;
      return cast(bool)s;
    }
    /* int to bool */
    else if (13 == testno)
    {
      int i = -20;
      return cast(bool)(i+20);
    }
    else if (14 == testno)
    {
      int i = -20;
      return cast(bool)i;
    }
    /* uint to bool */
    else if (15 == testno)
    {
      uint u;
      return cast(bool)u;
    }
    else if (16 == testno)
    {
      uint u = 314;
      return cast(bool)u;
    }

    return false;
  }

  /* casts to char */
  char
  cast_to_char(uint testno)
  {
    char res;

    /* bool to char */
    if (testno == 0)
    {
      bool b = true;
      res = cast(char)b;
    }
    else if (testno == 1)
    {
      bool b = false;
      res = cast(char)b;
    }

    /* char to char */
    else if (testno == 2)
    {
      char c;
      res = cast(char)c;
    }
    else if (testno == 3)
    {
      char c = '\n';
      res = cast(char)c;
    }
    else if (testno == 4)
    {
      char c = 'b';
      res = cast(char)c;
    }

    /* byte to char */
    else if (testno == 5)
    {
      byte b = 42;
      res = cast(char)b;
    }
    else if (testno == 6)
    {
      byte b = -1;
      res = cast(char)b;
    }

    /* ubyte to char */
    else if (testno == 7)
    {
      ubyte b = 127;
      res = cast(char)b;
    }
    else if (testno == 8)
    {
      ubyte b = 39;
      res = cast(char)b;
    }

    /* short to char */
    else if (testno == 9)
    {
      short s = 0x2A40;
      res = cast(char)s;
    }
    else if (testno == 10)
    {
      short s = -30100;
      res = cast(char)s;
    }

    /* ushort to char */
    else if (testno == 11)
    {
      short u = 1024 + 32;
      res = cast(char)u;
    }

    /* int to char */
    else if (testno == 12)
    {
      int i = -3;
      res = cast(char)i;
    }
    /* uint to char */
    else if (testno == 13)
    {
      uint i = 0xaabbcc30;
      res = cast(char)i;
    }

    return res;
  }

  /* casts to byte */
  byte
  cast_to_byte(uint testno)
  {
    byte res;

    /* bool to byte */
    if (testno == 0)
    {
       bool b = true;
       res = cast(byte)b;
    }
    else if (testno == 1)
    {
       bool b = false;
       res = cast(byte)b;
    }

    /* char to byte */
    else if (testno == 2)
    {
       char c = '#';
       res = cast(byte)c;
    }
    else if (testno == 3)
    {
       char c = 'm';
       res = cast(byte)c;
    }

    /* byte to byte */
    else if (testno == 4)
    {
       byte b = -9;
       res = cast(byte)b;
    }
    else if (testno == 5)
    {
       byte b = 102;
       res = cast(byte)b;
    }

    /* ubyte to ubyte */
    else if (testno == 6)
    {
       ubyte b = 67;
       res = cast(byte)b;
    }
    else if (testno == 7)
    {
       ubyte b = 199;
       res = cast(byte)b;
    }

    /* short to byte */
    else if (testno == 8)
    {
      short s;
      res = cast(byte)s;
    }
    else if (testno == 9)
    {
      short s = 0x1234;
      res = cast(byte)s;
    }

    /* ushort to byte */
    else if (testno == 10)
    {
      ushort s = 127;
      res = cast(byte)s;
    }
    else if (testno == 11)
    {
      ushort s = 130;
      res = cast(byte)s;
    }

    /* int to byte */
    else if (testno == 12)
    {
      int i = -1;
      res = cast(byte)i;
    }
    else if (testno == 13)
    {
      int i = 42;
      res = cast(byte)i;
    }

    /* uint  to byte */
    else if (testno == 14)
    {
      uint u = 120;
      res = cast(byte)u;
    }
    else if (testno == 15)
    {
      uint u = 0x11223344;
      res = cast(byte)u;
    }

    return res;
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

    /* short to ubyte */
    else if (8 == testno)
    {
      short s = -3;
      return cast(ubyte)s;
    }
    else if (9 == testno)
    {
      short s = 31;
      return cast(ubyte)s;
    }

    /* ushort to ubyte */
    else if (10 == testno)
    {
      ushort u;
      return cast(ubyte)u;
    }
    else if (11 == testno)
    {
      ushort u = 0xaaf0;
      return cast(ubyte)u;
    }

    /* int to ubyte */
    else if (12 == testno)
    {
      int i = 243;
      return cast(ubyte)i;
    }
    else if (13 == testno)
    {
      int i = 258;
      return cast(ubyte)i;
    }

    /* uint to ubyte */
    else if (14 == testno)
    {
      uint i = 123;
      return cast(ubyte)i;
    }
    else if (15 == testno)
    {
      int i = 0xffffff06;
      return cast(ubyte)i;
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

    /* bool to short */
    else if (4 == testno)
    {
      bool b = true;
      return cast(short)b;
    }
    else if (5 == testno)
    {
      bool b = false;
      return cast(short)b;
    }

    /* char to short */
    else if (6 == testno)
    {
      char c;
      return cast(short)c;
    }
    else if (7 == testno)
    {
      char c = 'm';
      return cast(short)c;
    }

    /* ubyte to short */
    else if (8 == testno)
    {
      ubyte b;
      return cast(short)b;
    }
    else if (9 == testno)
    {
      ubyte b = 110;
      return cast(short)b;
    }

    /* int to short */
    else if (10 == testno)
    {
      int i = -200;
      return cast(short)i;
    }
    else if (11 == testno)
    {
      int i = 0x89abcdef;
      return cast(short)i;
    }

    /* uint to short */
    else if (12 == testno)
    {
      uint u = 14234;
      return cast(short)u;
    }
    else if (13 == testno)
    {
      uint u = 32769;
      return cast(short)u;
    }

    /* byte to short */
    else if (14 == testno)
    {
      byte b = -10;
      return cast(short)b;
    }
    else if (15 == testno)
    {
      byte b = 34;
      return cast(short)b;
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

    /* bool to ushort */
    else if (4 == testno)
    {
      bool b = true;
      return cast(ushort)b;
    }
    else if (5 == testno)
    {
      bool b = false;
      return cast(ushort)b;
    }

    /* char to ushort */
    else if (6 == testno)
    {
      char c;
      return cast(ushort)c;
    }
    else if (7 == testno)
    {
      char c = ' ';
      return cast(ushort)c;
    }

    /* ubyte to ushort */
    else if (8 == testno)
    {
      ubyte b = 23;
      return cast(ushort)b;
    }
    else if (9 == testno)
    {
      ubyte b = 123;
      return cast(ushort)b;
    }

    /* int to ushort */
    else if (10 == testno)
    {
      int i = 23000;
      return cast(ushort)i;
    }
    else if (11 == testno)
    {
      int i = 65538;
      return cast(ushort)i;
    }

    /* uint to ushort */
    else if (12 == testno)
    {
      uint u = 67;
      return cast(ushort)u;
    }
    else if (13 == testno)
    {
      uint u = 0xffff000f;
      return cast(ushort)u;
    }

    /* byte to ushort */
    else if (14 == testno)
    {
      byte b = -7;
      return cast(ushort)b;
    }
    else if (15 == testno)
    {
      byte b = 4;
      return cast(ushort)b;
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

    /* bool to int */
    else if (4 == testno)
    {
      bool b = true;
      return cast(int)b;
    }
    else if (5 == testno)
    {
      bool b = false;
      return cast(int)b;
    }

    /* char to int */
    else if (6 == testno)
    {
      char c = '\n';
      return cast(int)c;
    }
    else if (7 == testno)
    {
      char c;
      return cast(int)c;
    }

    /* ubyte to int */
    else if (8 == testno)
    {
      ubyte b = 3;
      return cast(int)b;
    }
    else if (9 == testno)
    {
      ubyte b = 254;
      return cast(int)b;
    }

    /* ushort to int */
    else if (10 == testno)
    {
      ushort s = 300;
      return cast(int)s;
    }
    else if (11 == testno)
    {
      ushort s = 257;
      return cast(int)s;
    }

    /* byte to int */
    else if (12 == testno)
    {
      byte b = -100;
      return cast(int)b;
    }
    else if (13 == testno)
    {
      byte b = 56;
      return cast(int)b;
    }

    /* short to int */
    else if (14 == testno)
    {
      short c = -9;
      return cast(int)c;
    }
    else if (15 == testno)
    {
      short c = 9;
      return cast(int)c;
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

    /* bool to uint */
    else if (4 == testno)
    {
      bool b = true;
      return cast(uint)b;
    }
    else if (5 == testno)
    {
      bool b = false;
      return cast(uint)b;
    }

    /* char to uint */
    else if (6 == testno)
    {
      char c;
      return cast(uint)c;
    }
    else if (7 == testno)
    {
      char c = 'f';
      return cast(uint)c;
    }

    /* ubyte to uint */
    else if (8 == testno)
    {
      ubyte b = 254;
      return cast(uint)b;
    }
    else if (9 == testno)
    {
      ubyte b = 56;
      return cast(uint)b;
    }

    /* ushort to uint */
    else if (10 == testno)
    {
      ushort b = 1234;
      return cast(uint)b;
    }
    else if (11 == testno)
    {
      ushort b;
      return cast(uint)b;
    }

    /* byte to uint */
    else if (12 == testno)
    {
      byte b = -120;
      return cast(uint)b;
    }
    else if (13 == testno)
    {
      byte b = 119;
      return cast(uint)b;
    }

    /* short to uint */
    else if (14 == testno)
    {
      short c = 1234;
      return cast(uint)c;
    }
    else if (15 == testno)
    {
      short c = -1;
      return cast(uint)c;
    }


    return 0;
  }

  /* cast to void* */
  void *
  cast_to_void_ptr(uint testno)
  {
    void *p = cast(void*)-1;

    /* bool to void* */
    if (0 == testno)
    {
      p = cast(void*)true;
    }
    else if (1 == testno)
    {
      bool b = false;
      p = cast(void*)b;
    }

    /* char to void* */
    else if (2 == testno)
    {
      p = cast(void*)'R';
    }
    else if (3 == testno)
    {
      char c;
      p = cast(void*)c;
    }

    /* byte to void* */
    else if (4 == testno)
    {
      byte b = 32;
      p = cast(void*)b;
    }

    /* ubyte to void* */
    else if (5 == testno)
    {
      ubyte b = 65;
      p = cast(void*)b;
    }

    /* short to void* */
    else if (6 == testno)
    {
      short s = -5;
      p = cast(void*)s;
    }

    /* ushort to void* */
    else if (7 == testno)
    {
      ushort s = 15;
      p = cast(void*)s;
    }

    /* int to void* */
    else if (8 == testno)
    {
      p = cast(void*)0xaabb;
    }
    else if (9 == testno)
    {
      int i = 4;
      p = cast(void*)i;
    }

    /* uint to void* */
    else if (10 == testno)
    {
      uint u = 16;
      p = cast(void*)u;
    }

    return p;
  }

  /* cast to int* */
  int *
  cast_to_int_ptr(uint testno)
  {
    int *p;

    /* bool to int* */
    if (0 == testno)
    {
      p = cast(int*)true;
    }

    /* char to int* */
    else if (1 == testno)
    {
      p = cast(int*)'Q';
    }
    else if (2 == testno)
    {
      char c;
      p = cast(int*)c;
    }

    /* uint to int* */
    else if (3 == testno)
    {
      uint u = 123;
      p = cast(int*)u;
    }

    /* void* to int* */
    else if (4 == testno)
    {
      void *v = cast(void*)0xcdef;
      p = cast(int*)v;
    }

    /* short* to int* */
    else if (5 == testno)
    {
      short *s = cast(short*)0xfedc;
      p = cast(int*)s;
    }

    return p;
  }
}

