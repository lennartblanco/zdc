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

enum nums // base type char
{
  ZERO = '0',
  ONE,
  TWO,
  FIVE = '5',
  SIX,
  SEVEN,
  EIGHT
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

enum logic // base type bool
{
  T = true,
  F = false
}

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


char
nums_get_def_as_char()
{
   nums x;

   return x;
}

char
nums_get_SIX_as_char()
{
   nums x = nums.SIX;

   return x;
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
logic_get_def_as_bool()
{
   logic x;

   return x;
}

bool
logic_get_F_as_bool()
{
   return logic.F;
}

bool
unnamed_enum_param(bool dummy, nums)
{
    return !dummy;
}