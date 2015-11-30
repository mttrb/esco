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
#include <iostream>
#include <getopt.h>
#include <algorithm>
#include "consoleface.h"
#include "escoconf.h"
#include "machinecreater.h"
#include "filereader.h"
#include "termenv.h"
#include "strerror.h"

using namespace std;

/* INTERACT_WITH_USER - this feature is just for stupid windows users who don't know how to run console apps */

ConsoleFace::ConsoleFace() : quiet(false) {

}

ConsoleFace::~ConsoleFace() {
}

void arguments_fix(int arg, int *argc, char ***argv) {
    copy(*argv+arg+1, *argv+*argc+1, *argv+arg);
    --*argc;
}


bool ConsoleFace::init(int *argc, char*** argv) {
    /* params: -q */
    struct option longopts[] = {
        {"quiet", no_argument, NULL, 'q'},
        {NULL, 0, NULL, 0}
    };
    optind = 1;
    opterr = 0;
    int c;
    while ((c = getopt_long(*argc, *argv, "-:q", longopts, NULL)) != -1) {
        switch (c) {
        case 'q':
            /* quiet mode */
            quiet = true;
            --optind;
            arguments_fix(optind, argc, argv);
            break;
        /* ignore other options */
        default:
            break;
        }
    }
    return true;
}

const char *ConsoleFace::getUsage() {
    return "--quiet (-q) - be quiet (don't output garbage)";
}

bool ConsoleFace::build() {
#ifndef INTERACT_WITH_USER
    if (!conf.getfile()) {
        conf.head();
        cerr<<"Script to execute not specified.\n";
        return false;
    }
#endif
    if (conf.gettype() == TYPE_UNDEF)
        conf.settype(conf.determine_mtype(conf.getfile()));
#ifndef INTERACT_WITH_USER
    if (conf.gettype() == TYPE_UNKNOW) {
        conf.head();
        cerr<<"Unknown language\n";
        return false;
    }
#endif
        
    return true;
}

int ConsoleFace::run() {
#ifdef INTERACT_WITH_USER
    bool pause = false;

    if (!conf.getfile()) {

        cout<<"Script to execute not specified.\n";
        cout<<"Please enter path to script: ";
        char tptr[256];
        cin.getline(tptr, sizeof(tptr));
        conf.setfile(tptr);
        pause = true;
    }
#endif

    mtype mt = 
#ifndef INTERACT_WITH_USER
        conf.gettype();
#else
    register bool doexit = false;
    do {
        switch (mt = conf.gettype()) {
            case TYPE_UNDEF:
            case TYPE_UNKNOW:

                cout<<"No such language\n";

                cout<<"Please specify script's language: ";           
                char szmtype[256];
                cin.getline(szmtype, sizeof szmtype);

                conf.setchecktype(szmtype);
                
                pause = true;
            default:
                doexit = true;
        }
    } while (!doexit);
#endif

    SourceReader* sr = new FileReader(conf.getfile());

    OutputEnv *out = (oenv)?oenv:new OutputFile(stdout);
    CompositeEnv *et = new CompositeEnv(
            (NULL != ienv)?ienv : new InputFile(stdin),
            out);

    /* TODO: change this */
    MachineCreater mc(mt, sr, et);

    Machine* em = mc.create();
    if (em == 0) {
        cerr<<"Error during machine creating\n";
        cerr<<"Error: "<<rerr2str(mc.getError())<<".\n";
        exit(1);
    }

    if (! quiet ) cout<<"== Begin of execution ==\n";
    int ret = STEP_EXEC_OK;
    do {
        ret = em->step();
    }
    while (ret & STEP_EXEC_OK);

    
    if (conf.getdebug()) {
        cout<<"\n\nExit code:\""<<ret<<"\"\n";
        cout<<eerr2str(ret);
    }
    else {
        if (! quiet ) cout<<"\n== End of execution ==\n";
    }
    delete em;
    delete et;
    
    #ifdef INTERACT_WITH_USER
    if (pause) {
       while (cin.get() != '\n')
       {};
    }
    #endif

    return EXIT_SUCCESS;
}


