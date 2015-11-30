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
#include "compositeenv.h"

InputEnv::~InputEnv() {};
OutputEnv::~OutputEnv() {};

DoubleOutputEnv::DoubleOutputEnv(OutputEnv &_out0, OutputEnv &_out1) :
            out0(_out0), out1(_out1) {

}

DoubleOutputEnv::~DoubleOutputEnv() {

}

void DoubleOutputEnv::putChar(char c) {
    out0.putChar(c);
    out1.putChar(c);
}

void DoubleOutputEnv::putStr(const char *str) {
    out0.putStr(str);
    out1.putStr(str);
}

CompositeEnv::CompositeEnv(InputEnv *_in, OutputEnv *_out) :
            in(_in), out(_out) {

}

CompositeEnv::~CompositeEnv() {
    delete in;
    delete out;
}

bool InputFile::getStr(char* s, int l) {
    return fgets(s, l, istream) != NULL;
};



