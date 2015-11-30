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

#include <config.h>
#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <cstring>
#include "termenv.h"

TerminalEnv::TerminalEnv(FILE *_istream, FILE *_ostream) : istream(_istream), ostream(_ostream){
};

TerminalEnv::~TerminalEnv() {
    fclose(istream);
    fclose(ostream);

}

char TerminalEnv::getChar() {
    return getc(istream);
}

void TerminalEnv::putChar(char c) {
    putc(c, ostream);
    // to Zasenko Sergey: flushing is really neccessary, 
    // because output gets visible only after a newline, 
    // and such things like (on ONE line):
    // "enter a number: "
    // aren't displayed at all. 
    // That would be very confusing for users, how 
    // should they know what they have to do?
    fflush(ostream);
};

void TerminalEnv::putStr(const char* str) {
    fputs(str, ostream);
    fflush(ostream);
}

void TerminalEnv::getStr(char* str, int n) {
    fgets(str, n, istream);
}




