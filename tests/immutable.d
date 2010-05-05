int immutable_int(uint var_num, uint idx)
{
  immutable int v1 = 20;
  immutable (int) v2;
  immutable (int[]) v3 = [-10, -20, -30];
  immutable (int)[] v4 = [1, 2, 3];

  if (var_num == 1)
  {
    return v1;
  }
  else if (var_num == 2)
  {
    return v2;
  }
  else if (var_num == 3)
  {
    return v3[idx];
  }
  else if (var_num == 4)
  {
    return v4[idx];
  }

  return -1;
}

bool immutable_bool(int x)
{
  immutable bool t = true;
  immutable bool f = false;

  if (x > 0) { return t; }
  return f;
}

bool char_present(immutable(char)[] str, immutable char c)
{
  uint i = 0;
  while (i < str.length)
  {
    if (str[i] == c)
    {
      return true;
    }
    i = i + 1;
  }

  return false;
}

bool invoke_char_present(char c)
{
  return char_present(['p', 't', 's', 'c', 'q'], c);
}
