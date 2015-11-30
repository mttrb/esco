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
#include "bytermachine.h"
#include "escoconf.h"

const char* byt_cmds = "0<>VA{}+-$#";

ByterMachine::operation ByterMachine::functions = {
    &ByterMachine::PrevCell, &ByterMachine::MoveLeft,
    &ByterMachine::MoveRight, &ByterMachine::MoveDown,
    &ByterMachine::MoveUp, &ByterMachine::PrintLeft,
    &ByterMachine::PrintRight, &ByterMachine::PrintUp,
    &ByterMachine::PrintDown,&ByterMachine::PrintBegin,
    &ByterMachine::Exit
};

ByterMachine::ByterMachine(Source* src, EnvTouch* et) : 
    x(0), y(0), px(0), py(0)
{
    bm = reinterpret_cast<ByterMemory*>(src);
    this->et = et;

    if (conf.getdebug()) {
        for (int i=0; i < SIZE_Y; i++) {
            for (int j = 0; j < SIZE_X; j++)
                putchar(byt_cmds[bm->getCell(i,j)]);
            puts("\n");
        }
    }
}

ByterMachine::~ByterMachine() {
    delete bm;
}

int ByterMachine::step() {
    int op = (int)bm->getCell(y,x);
    if (op < byt_num_of_cmd) {
        dbgprint("Executing command %c\n", byt_cmds[op]);
        return (this->*functions[op])();
    }
    else {
        dbgprint("Unknow opcode %u\nExiting...\n",op);
        return STEP_EXEC_ERR;
    }
}
int ByterMachine::PrevCell() {
    x = px;
    y = py;
    return STEP_EXEC_OK;
}

int ByterMachine::MoveLeft() {
    bm->setCell(y,x,byt_move_right);
    decX();
    return STEP_EXEC_OK;
}

int ByterMachine::MoveRight() {
    bm->setCell(y,x,byt_move_left);
    incX();
    return STEP_EXEC_OK;
}

int ByterMachine::MoveDown() {
    bm->setCell(y,x,byt_move_up);
    incY();
    return STEP_EXEC_OK;
}

int ByterMachine::MoveUp() {
    bm->setCell(y,x,byt_move_down);
    decY();
    return STEP_EXEC_OK;
}

int ByterMachine::PrintLeft() {
    putChar();
    decX();
    return STEP_EXEC_OK;
}

int ByterMachine::PrintRight() {
    putChar();
    incX();
    return STEP_EXEC_OK;
}

int ByterMachine::PrintUp() {
    putChar();
    decY();
    return STEP_EXEC_OK;
}

int ByterMachine::PrintDown() {
    putChar();
    incY();
    return STEP_EXEC_OK;
}

int ByterMachine::PrintBegin() {
    putChar();
    x=0;
    y=0;
    return STEP_EXEC_OK;
}

int ByterMachine::Exit() {
    return STEP_EXEC_END;
}

/* For future extension */

const char* ByterMachine::getCode() {
    fprintf(stderr, "Add sourcecode to Byter... But byter have nothing except memory - think about it...\n");
    return NULL;
}

int ByterMachine::getDbg() {
    fprintf(stderr, "Add debug information to Byter\n");
    return 0;
}

int ByterMachine::getDbgSize() {
    return 0;
}



