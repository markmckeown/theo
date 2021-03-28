/**
    Copyright 2021 WANdisco
    Author Mark Mc Keown.

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
    along with Libtheo.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef CHUNKER_H
#define CHUNKER_H

#define CHUNKER_WINDOW_WIDTH 32
#define CHUNKER_MIN_CHUNK_SIZE 2 * 1024
#define CHUNKER_MAX_CHUNK_SIZE 24 * 1024
#define CHUNKER_AVG_CHUNK_SIZE 8 * 1024
#define CHUNKER_MASK           8191
#define CHUNKER_TRIGGER        26061973 & CHUNKER_MASK

#define CHUNKER_FOUND          0
#define CHUNKER_NOT_FOUND      -1

#include <stdint.h>
#include <stdbool.h>

#include "isa-l_crypto.h"

struct chunk_ref;

struct chunker {
	struct    rh_state2 rh_state2;
	char      retained[CHUNKER_MAX_CHUNK_SIZE];
	uint32_t   retained_size;
};


int chunker_init(struct chunker *chunker);

int chunker_chunk_buffer(struct chunker *chunker,
                struct chunk_ref *chunk_ref,
                char *buffer,
                uint32_t buffer_length);

uint32_t chunker_flush_retained(struct chunker *chunker, 
		char *buffer, uint32_t buffer_length);


#endif
