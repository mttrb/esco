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

#ifndef MACHINECREATER_H
#define MACHINECREATER_H

#include "machine.h"
#include "preprocessor.h"
#include "envtouch.h"
#include "escoconf.h"

class MachineCreater {
    public:
        /* 1st parametr - machine type (see escocong.h)
         * 2d parametr - source reader
         * 3d parametr - interface witch interact with envirioment
         */
        MachineCreater(mtype,SourceReader*,EnvTouch*);
        ~MachineCreater();

        // Create machine
        Machine* create(void);
        int getError(void);
    private:
        EnvTouch* et;
        SourceReader* sr;
        PreProcessor* pr;
        int error;
        mtype machine;
};

#endif




