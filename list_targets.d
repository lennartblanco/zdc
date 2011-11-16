#!/usr/bin/rdmd -J.

import std.stdio;
import targets;

void main()
{
  auto targets = mixin(import("config"));
  bool space = false;
  foreach (target; targets.keys)
  {
    write(space ? " " : "", target);
    space = true;
  }
}
