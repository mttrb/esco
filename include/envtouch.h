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

/* Declarate interface for machine' IO operations */

#ifndef ENVTOUCH_H
#define ENVTOUCH_H

#include <config.h>

class EnvTouch {
    public:
        virtual ~EnvTouch();

        virtual char getChar() = 0;
        virtual void putChar(char) = 0;

        virtual void getStr(char*, int) = 0;
        virtual void putStr(const char*) = 0;
};

#endif








