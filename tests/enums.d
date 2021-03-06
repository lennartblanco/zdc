enum states // base type int
{
  idle,
  run,
  stop
}

enum ustates // base type uint
{
  on = 10u,
  off,
  disabled
}

enum nato : char
{
  UNKNOW,      //   0
  ABORT,       //   1
  ALPHA = 'a', //   97
  BRAVO,       //   98
  CHARLIE,     //   99
  DELTA,       //   100
  ECHO         //   101
}

enum emot : immutable(char)[]
{
 happy = "yellow",
 angry = "green",
 sad = "blue"
}

alias int myint;
enum myenum : myint { a, b, c }  // user base type

enum ANON_ENUM = 23;
enum AE_A = 20, AE_B = 21, AE_C = 22;
enum { E = 100, N = 'c', U = "orange", M = false }

enum DEFAULT_COLOR = "blue";

int
states_get_def_as_int()
{
    states x;

    return x;
}

int
states_get_idle_as_int()
{
   states x = states.idle;

   return x;
}

int
states_get_stop_as_int()
{
   return states.stop;
}

int
states_get_run_as_int()
{
   states x;
   x = states.run;

   return x;
}

states
get_state(char t)
{
    if (t == 'r')
    {
        return states.run; 
    }
    else if (t == 's')
    {
         return states.stop;
    }

    return states.idle;
}

extern (C) uint
ustates_get_sizeof()
{
    return ustates.sizeof;
}

uint
ustates_get_def_as_uint()
{
    ustates x;

    return x;
}

uint
ustates_get_off_as_uint()
{
    return ustates.off;
}

uint
ustates_get_as_uint(ustates x)
{
    return x;
}

extern (C) uint
nato_get_sizeof()
{
    return nato.sizeof;
}

char
nato_get_def_as_char()
{
  nato x;

  return x;
}

char
nato_get_DELTA_as_char()
{
  nato x;

  x = nato.DELTA;

  return x;
}

nato
get_next_char(nato n)
{
    if (n == nato.ALPHA)        { return nato.BRAVO;   }
    else if (n == nato.BRAVO)   { return nato.CHARLIE; }
    else if (n == nato.CHARLIE) { return nato.DELTA;   }
    else if (n == nato.DELTA)   { return nato.ECHO;    }

    return nato.UNKNOW;
}

bool
unnamed_enum_param(bool dummy, nato)
{
    return !dummy;
}

extern (C) int
myenum_as_int(char n)
{
  myenum res;

  if (n == 'a')
  {
    res = myenum.a;
  }
  else if (n == 'b')
  {
    res = myenum.b;
  }
  else if (n == 'c')
  {
    res = myenum.c;
  }

  return res;
}

extern (C) int anon_enum_val()
{
  return ANON_ENUM;
}

extern (C) int get_ae_exp(int i)
{
  int sum = AE_A;

  sum = sum + AE_B;

  if (i > 0) { return (sum + AE_C) * i; }
  else       { sum = sum + (i + AE_C); }

  return sum;
}


extern (C) int get_anon_e() { return E; }

extern (C) uint get_u_length(bool via_var)
{
  if (via_var)
  {
     immutable(char)[] str = U;

     return str.length;
  }

  return U.length;
}

extern (C) char get_def_color_c(uint idx)
{
  if (idx >= DEFAULT_COLOR.length)
  {
    return '?';
  }

  immutable(char)[] str = DEFAULT_COLOR;
  return str[idx];
}

extern (C) int emot_num(int a)
{
  immutable(char)[] str;
  int num = 0;

  if (a == 0) { str = emot.happy; }
  else if (a == 1) { str = emot.angry; }
  else { str = emot.sad; }

  foreach (c; str)
  {
    num = num + c;
  }

  return num;
}

/*
 * wrappers to allow call test functions with C calling convention
 */
extern (C)
{
  int
  call_states_get_def_as_int()
  {
    return states_get_def_as_int();
  }

  int
  call_states_get_idle_as_int()
  {
    return states_get_idle_as_int();
  }

  int
  call_states_get_stop_as_int()
  {
    return states_get_stop_as_int();
  }

  int
  call_states_get_run_as_int()
  {
    return states_get_run_as_int();
  }

  int
  call_get_state(char arg)
  {
    return get_state(arg);
  }

  uint
  call_ustates_get_def_as_uint()
  {
    return ustates_get_def_as_uint();
  }

  uint
  call_ustates_get_off_as_uint()
  {
    return ustates_get_off_as_uint();
  }

  uint
  call_ustates_get_as_uint(ustates arg)
  {
    return ustates_get_as_uint(arg);
  }

  char
  call_nato_get_def_as_char()
  {
    return nato_get_def_as_char();
  }

  char
  call_nato_get_DELTA_as_char()
  {
    return nato_get_DELTA_as_char();
  }

  nato
  call_get_next_char(nato arg)
  {
    return get_next_char(arg);
  }

  bool
  call_unnamed_enum_param(bool arg1, nato arg2)
  {
    return unnamed_enum_param(arg1, arg2);
  }
}
