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

#include "theo/chunk.h"
#include "theo/msys.h"


void chunk_init(struct chunk *chunk) {
	memset(chunk, 0, sizeof(struct chunk));
	checksum_init(&chunk->checksum);
	return;
}

void chunk_set(struct chunk *chunk, struct checksum *checksum, char *buffer,
		uint32_t buffer_size) {
	ALWAYS(buffer_size <= CHUNK_MAX_BUFFER_SIZE);
	memcpy(&chunk->checksum, checksum, sizeof(struct checksum));
	memcpy(chunk->buffer, buffer, buffer_size);
	chunk->buffer_size = buffer_size;
	return;
}
