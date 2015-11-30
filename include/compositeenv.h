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

/* This is using to allow read from one type of environment and write to other */

#ifndef _COMPOSITEENV_H_
#define _COMPOSITEENV_H_

#include <config.h>
#include <cstdio>
#include "envtouch.h"

class InputEnv {
    public:
        virtual ~InputEnv();
        virtual char getChar() = 0;
        virtual bool getStr(char*, int) = 0;
};

class OutputEnv {
    public:
        virtual ~OutputEnv();
        virtual void putChar(char) = 0;
        virtual void putStr(const char*) = 0;
};

/* To output to two different environments (for example to file and to stdout) */
class DoubleOutputEnv : public OutputEnv {
    public:
        DoubleOutputEnv(OutputEnv &_out0, OutputEnv &_out1);
        ~DoubleOutputEnv();

        void putChar(char);
        void putStr(const char*);

    private:
        OutputEnv &out0, &out1;

};

class CompositeEnv : public EnvTouch {
    public:
        CompositeEnv(InputEnv *_in, OutputEnv *_out);

        ~CompositeEnv();

        char getChar() {return in->getChar();};
        void putChar(char c) {out->putChar(c);};

        void getStr(char* str, int size) {in->getStr(str, size);};
        void putStr(const char* str) {out->putStr(str);};

    private:
        InputEnv *in;
        OutputEnv *out;
};

/* Some realizations of classes */
class InputFile : public InputEnv {
    public:
        InputFile(FILE* stream) : istream(stream) {};
        ~InputFile() {fclose(istream);};

        char getChar(void) {return fgetc(istream);};
        bool getStr(char* s, int l);
    private:
        FILE *istream;
};

class OutputFile : public OutputEnv {
    public:
        OutputFile(FILE* stream) : ostream(stream) {};
        ~OutputFile() {fclose(ostream);};

        inline void putChar(char);
        inline void putStr(const char *str);

    private:
        FILE *ostream;
};

void OutputFile::putChar(char c) {
    fputc(c, ostream);
    fflush(ostream);
}

void OutputFile::putStr(const char *str) {
    fputs(str, ostream);
    fflush(ostream);
}

#endif



