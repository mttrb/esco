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

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "compositeenv.h"

class Interface {
    public:
        Interface() : ienv(NULL), oenv(NULL) {};
        virtual ~Interface();

        static Interface& getPreferFace(void);

        /* init() method takes pointers to program' arguments */
        virtual bool init(int *argc, char ***argv) = 0;
        /* describes specific arguments of interface */
        virtual const char *getUsage() = 0;
        virtual bool build() = 0;
        virtual int run() = 0;
        
        /* Additional set* methods */
        virtual void setienv(InputEnv* env)  {ienv = env;};
        virtual void setoenv(OutputEnv* env) {oenv = env;};
    protected:
        /* They will be deleted by child class (by its CompositeEnv) */
        InputEnv *ienv;
        OutputEnv *oenv;
};

#endif



