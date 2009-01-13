/*
 *   entire.c
 *
 *   ENTIRE parser module API declarations
 *
 *   ENTIRE is a Generic Parser for the Entire Class of Context-Free Grammars.
 *
 *   Copyright (C) 1984, 1999, 2006 Friedrich Wilhelm Schroeer
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   ENTIRE may also be distributed as part of proprietary software
 *   if the distributor has obtained an extended license from
 *   Metarga GmbH, Joachim-Friedrich-Str. 54, D-10711 Berlin,
 *   info@metarga.de
 *
 */

#ifndef ENTIRE_INC_X
#define ENTIRE_INC_X

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

int yyparse();
void yymallocerror();

#endif /* ENTIRE_INC_X */
