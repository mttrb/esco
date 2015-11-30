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
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include "escoconf.h"

using namespace std;

escoconf::escoconf() : \
        face(NULL), debug(false), type(TYPE_UNDEF),\
        file(NULL), ifile(NULL), ofile(NULL)
    {
};

escoconf::~escoconf() {
    if (NULL != file)
        delete [] file;
    if (NULL != ifile)
        delete [] ifile;
    if (NULL != ofile)
        delete [] ofile;
}

void escoconf::head() {
    cout<<PACKAGE_STRING<<" - emulator for a lot (in future) esoteric languages.\nCopyright (C) 2007-2011, Zasenko Sergey\n\n";
}

void escoconf::usage(void) {
    head();
    params();
    cout<<face->getUsage()<<endl;
}

void escoconf::params() {
    cout<<"usage:\n";
    cout<<"esco [options] file\n";
    cout<<"Options:\n";
    cout<<"--help (-h) to read this\n";
    cout<<"--type (-t) <machine_type>\n";
#ifdef ENABLE_BRAINFUCK
    cout<<"\tbf - BrainFuck machine\n\took - Ook machine\n\tspoon - Spoon machine\n\thodor - Hodor machine\n\tXD - XD machine\n";
#endif
#ifdef ENABLE_HQ9
    cout<<"\thq9+ - HQ9+ machine\n";
#endif
#ifdef ENABLE_WHITESPACE
    cout<<"\tws - Whitespace machine\n";
#endif
#ifdef ENABLE_BEFUNGE
    cout <<"\tb93 - Befunge-93 machine\n";
    /* cout <<"\tb98 - Befunge-98 machine\n"; <- We have no befunge-98 interpreter */
#endif
#ifdef ENABLE_BYTER
    cout <<"\tbyter - Byter machine\n";
#endif
    cout<<"--input (-i) <file> - input from file\n";
    cout<<"--output (-o) <file> - output to file\n";
    cout<<"--debug (-d) - output debug information\n";
}

mtype escoconf::checkMachineType(const char* str) {
#ifdef ENABLE_BRAINFUCK
    if (!strcmp(str, "bf") || !strcmp(str, "brainfuck"))
        return TYPE_BRAINFUCK;
    if (!strcmp(str, "ook"))
        return TYPE_OOK;
    if (!strcmp(str, "spoon"))
        return TYPE_SPOON;
    if (!strcmp(str, "hodor"))
        return TYPE_HODOR;
    if (!strcmp(str, "xd"))
        return TYPE_XD;
#endif
#ifdef ENABLE_HQ9
    if (!strcmp(str, "hq9+"))
        return TYPE_HQ;
#endif
#ifdef ENABLE_WHITESPACE
    if (!strcmp(str, "ws"))
        return TYPE_WS;
#endif
#ifdef ENABLE_BEFUNGE
    if (!strcmp(str, "b93"))
        return TYPE_B93;
    if (!strcmp(str, "b98"))
        return TYPE_B98;
#endif
#ifdef ENABLE_BYTER
    if (!strcmp(str, "byter"))
        return TYPE_BYTER;
#endif
    return TYPE_UNKNOW;
}


Interface *escoconf::getInterface( void ) {
    if (! face )
        face = &Interface::getPreferFace();
    return face;
}

bool escoconf::commandline(int argc, char *argv[]) {
    struct option longopts[] = {
        { "help",   no_argument,        NULL,   'h'},
        { "type",   required_argument,  NULL,   't'},
        { "input",  required_argument,  NULL,   'i'},
        { "output", required_argument,  NULL,   'o'},
        { "debug",  no_argument,        NULL,   'd'},
        { 0,        0,                  0,      0  }
    };

    //Work with args
    char c;
    //opterr=1;
    optind = 1;
    while ((c = getopt_long(argc, (char* const*)argv, "-:ht:i:o:d", longopts, NULL)) != -1) {
        switch (c) {
            case 'd':
                debug = true;
                break;
            case 'h':
                usage();
                exit(0);
                break;
            case 't':
                type = checkMachineType(optarg);
                break;
            case 'i':
                setifile(optarg);
                break;
            case 'o':
                setofile(optarg);
                break;
            case 0:
                break;
            case 1:
                setfile(optarg);
                break;
            case ':':
                usage();
                cerr<<endl<<"Option '-"<<(char)optopt<<"' requires an argument"<<endl;
                return false;
                break;
            case '?':
            default:
                usage();
                cerr<<endl<<"Option '-"<<(char)optopt<<"' is invalid"<<endl;
                return false;
                break;
        }
    }
    return true;
}

void escoconf::setfile(const char* _file) {
    int size = strlen(_file)+1;//Plus NULL byte
    file = new char [size];
    strncpy(file, _file, size);
};

void escoconf::setifile(const char *_file) {
    int size = strlen(_file)+1;//Plus NULL byte
    ifile = new char [size];
    strncpy(ifile, _file, size);
}

void escoconf::setofile(const char *_file) {
    int size = strlen(_file)+1;//Plus NULL byte
    ofile = new char [size];
    strncpy(ofile, _file, size);
}

void escoconf::setdebug(bool d) {
    debug = d;
}

void escoconf::setchecktype(const char* str) {
    type = checkMachineType(str);
}

void escoconf::settype(mtype _type) {
    type = _type;
}

mtype escoconf::determine_mtype(const char* filename) {
    //file extension 2 machine type
    struct {
        const char* ext;
        mtype mt;
    }
    ext2mt[] = {
        #ifdef ENABLE_BRAINFUCK
        {"bf", TYPE_BRAINFUCK},
        {"ook", TYPE_OOK},
        {"spoon", TYPE_SPOON},
        {"hodor", TYPE_HODOR},
        {"xd", TYPE_XD},
        #endif
        #ifdef ENABLE_HQ9
        {"hq9", TYPE_HQ},
        #endif
        #ifdef ENABLE_WHITESPACE
        {"ws", TYPE_WS},
        #endif
        #ifdef ENABLE_BEFUNGE
	    { "b93", TYPE_B93 },
        { "b98", TYPE_B98 },
        #endif
        #ifdef ENABLE_BYTER
        { "byt", TYPE_BYTER},
        #endif
        {NULL, TYPE_UNKNOW}
    };

    const char* ext = strrchr(filename, '.');
    if (ext == NULL)
        return TYPE_UNKNOW;
    ++ext;
    int i = 0;
    while (ext2mt[i].mt != TYPE_UNKNOW) {
        if (!strcmp(ext, ext2mt[i].ext))
            break;
        ++i;
    }
    return ext2mt[i].mt;

}



