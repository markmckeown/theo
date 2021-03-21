/**
    Copyright 2021 WANdisco

    This file is part of Libtheo.

    Libtheo is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef CELL_IC_H
#define CELL_IC_H

#include <stdint.h>
#include <stdbool.h>

// Cell Integrity Code
struct cell_ic {
	uint32_t entry_code;
	uint32_t exit_code;
};


void cell_ic_init(struct cell_ic *cell_ic);

bool cell_ic_sane(struct cell_ic *cell_ic);

uint32_t cell_ic_on_entry(struct cell_ic *cell_ic);

uint32_t cell_ic_on_exit(struct cell_ic *cell_ic);

#endif
