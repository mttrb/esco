/* esco - interpreter for a lot (in future) esoteric languages.
 * Copyright (C) 2007-2011, Zasenko Sergey
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef B93DEFS_H
#define B93DEFS_H

enum {
	DIR_NORTH = 0, 
	DIR_EAST, 
	DIR_SOUTH, 
	DIR_WEST 
};

#define MAXX (80)
#define MAXY (25)

// opcodes

#define B93OP_ADD	(unsigned char)('+') 
#define B93OP_SUB	(unsigned char)('-') 
#define B93OP_MULT	(unsigned char)('*') 
#define B93OP_DIV	(unsigned char)('/') 
#define B93OP_MOD	(unsigned char)('%') 
#define B93OP_NOT	(unsigned char)('!') 
#define B93OP_GT	(unsigned char)('`') 
#define B93OP_EAST	(unsigned char)('>')
#define B93OP_WEST	(unsigned char)('<') 
#define B93OP_NORTH	(unsigned char)('^') 
#define B93OP_SOUTH	(unsigned char)('v') 
#define B93OP_RAND	(unsigned char)('?') 
#define B93OP_HORZIF	(unsigned char)('_') 
#define B93OP_VERTIF	(unsigned char)('|') 
#define B93OP_STRINGMODE (unsigned char)('\"') 
#define B93OP_DUP	(unsigned char)(':') 
#define B93OP_SWAP	(unsigned char)('\\') 
#define B93OP_POP	(unsigned char)('$') 
#define B93OP_PRINTN	(unsigned char)('.') 
#define B93OP_PRINTC	(unsigned char)(',') 
#define B93OP_SKIP	(unsigned char)('#') 
#define B93OP_GET 	(unsigned char)('g') 
#define B93OP_PUT	(unsigned char)('p') 
#define B93OP_INPUTN	(unsigned char)('&') 
#define B93OP_INPUTC	(unsigned char)('~') 
#define B93OP_EXIT	(unsigned char)('@')
#define B93OP_ZERO	(unsigned char)('0')
#define B93OP_ONE	(unsigned char)('1')
#define B93OP_TWO	(unsigned char)('2')
#define B93OP_THREE	(unsigned char)('3')
#define B93OP_FOUR	(unsigned char)('4')
#define B93OP_FIVE	(unsigned char)('5')
#define B93OP_SIX	(unsigned char)('6')
#define B93OP_SEVEN	(unsigned char)('7')
#define B93OP_EIGHT	(unsigned char)('8')
#define B93OP_NINE	(unsigned char)('9')
#define B93OP_NOP	(unsigned char)(' ')

#endif


