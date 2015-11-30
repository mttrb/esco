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

/* Main program - read source, create machine etc... */

#include <config.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include "escoconf.h"
#include "termenv.h"
#include "compositeenv.h"

escoconf conf;
Interface* face = NULL;

void exit_proc() {
    delete face;
}

int main (int argc, char* argv[]) {
    atexit(exit_proc);
    face = conf.getInterface();
    if (!face->init(&argc, &argv)) {
        dbgprint("Failure during face->init()\n");
        return EXIT_FAILURE;
    }

    if (!conf.commandline(argc, argv)) {
        return EXIT_FAILURE;
    }

    if (!face->build()) {
        return EXIT_FAILURE;
    }


    /* Choose EnvTouch and configurate it */

    if (conf.getifile()) {
        FILE *ifile = fopen(conf.getifile(), "rb");
        if (ifile == 0) {
            perror(conf.getifile());
            exit(EXIT_FAILURE);
        }
        face->setienv(new InputFile(ifile));
    }

    if (conf.getofile()) {
        FILE *ofile = fopen(conf.getofile(), "wb+");
        if (ofile == 0) {
            perror(conf.getofile());
            exit(EXIT_FAILURE);
        }
        face->setoenv(new OutputFile(ofile));
    }

    //face->setenv();

    return face->run();
}



