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
#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

#include "theo/checksum.h"

#define CHUNK_MAX_BUFFER_SIZE 32768

struct chunk {
	struct checksum checksum;
	char            buffer[CHUNK_MAX_BUFFER_SIZE];
	uint16_t        buffer_size;
};


void chunk_init(struct chunk *chunk);

void chunk_set(struct chunk *chunk, struct checksum *checksum, char *buffer,
		uint32_t buffer_size);

#endif
