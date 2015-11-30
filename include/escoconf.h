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

/*
 * escoconf - manipulate with parametrs of esco
 */

#ifndef ESCOCONF_H
#define ESCOCONF_H

#include "interface.h"

// Some types of machines
enum mtype {
#ifdef ENABLE_BRAINFUCK
    TYPE_BRAINFUCK = 1,
    TYPE_OOK = 2,
    TYPE_SPOON = 3,
    TYPE_HODOR = 4,
    TYPE_XD = 10,
#endif
#ifdef ENABLE_HQ9
    TYPE_HQ = 5,
#endif
#ifdef ENABLE_WHITESPACE
    TYPE_WS = 6,
#endif
#ifdef ENABLE_BEFUNGE
    TYPE_B93 = 7,
    TYPE_B98 = 8,
#endif
#ifdef ENABLE_BYTER
    TYPE_BYTER = 9,
#endif
    TYPE_UNDEF = 0,
    TYPE_UNKNOW = -1
};

class escoconf {
    public:
        escoconf();
        ~escoconf();

        bool commandline(int argc, char* argv[]);

        void head(void);
        void params(void);
        void usage(void);

        Interface *getInterface( void );

        const char* getfile() {return file;};
        void setfile(const char*);

        const char* getifile() {return ifile;};
        void setifile(const char*);

        const char* getofile() {return ofile;};
        void setofile(const char*);

        mtype gettype() {return type;};
        void settype(mtype);
        void setchecktype(const char*);

        inline bool getdebug() {return debug;};
        void setdebug(bool);

        /* Determine machine type by filename */
        mtype determine_mtype(const char*);

    private:
        Interface *face;
        mtype checkMachineType(const char *);

        bool debug;
        mtype type;
        char* file, *ifile, *ofile;
}

extern conf;

#define dbgprint if(conf.getdebug()) printf

#endif



