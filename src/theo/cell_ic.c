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
#include <string.h>

#include "theo/cell_ic.h"

void cell_ic_init(struct cell_ic *cell_ic)
{
	memset(cell_ic, 0, sizeof(struct cell_ic));
	return;
}

bool cell_ic_sane(struct cell_ic *cell_ic)
{
	return (cell_ic->entry_code == cell_ic->exit_code);
}

uint32_t cell_ic_on_entry(struct cell_ic *cell_ic)
{
	cell_ic->entry_code++;
	// Now before proceeding issue a full memory barrier
	__sync_synchronize();
	return cell_ic->entry_code;
}

uint32_t cell_ic_on_exit(struct cell_ic *cell_ic)
{
	// Flush everything done to memory using full memory barrier.
	__sync_synchronize();
	cell_ic->exit_code = cell_ic->entry_code;
	// Make sure exit_code is flushed to memory.
	__sync_synchronize();
	return cell_ic->exit_code;
}
