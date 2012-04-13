#!/usr/bin/rdmd -J.

import std.stdio;
import config;

void main()
{
  bool space = false;
  foreach (target; get_targets_names())
  {
    write(space ? " " : "", target);
    space = true;
  }
}
