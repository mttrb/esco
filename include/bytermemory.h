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

#ifndef _BYTER_MEMORY_H_
#define _BYTER_MEMORY_H_

const int SIZE_X = 16;
const int SIZE_Y = 16;

enum byter_cmd {
    byt_prev_cell,  // 0
    byt_move_left,  // <
    byt_move_right, // >
    byt_move_down,  // V
    byt_move_up,    // A
    byt_print_left, // {
    byt_print_right,// }
    byt_print_up,   // +
    byt_print_down, // -
    byt_print_begin,// $
    byt_exit,       // #
    byt_num_of_cmd

};

typedef byter_cmd byter_cell;

class ByterMemory {
    public:
        ByterMemory(byter_cell fill_sym = byt_prev_cell) {
            for (int i = 0; i < 16; i++)
                for (int j = 0; j < 16; j++)
                    memory[i][j] = fill_sym;
        };

        inline byter_cell getCell(int i, int j) { return memory[i][j]; }
        inline void setCell(int i, int j, byter_cell arg) {memory[i][j]=arg;}

    private:
        byter_cell memory[SIZE_X][SIZE_Y];
};

#endif



