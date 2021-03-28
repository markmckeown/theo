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
#ifndef CHUNK_STREAMER_H
#define CHUNK_STREAMER_H

#include "theo/chunker.h"

struct cache_manager;

struct chunk_streamer {
	struct    chunker        chunker;
	struct    cache_manager *cache_manager;
	uint32_t  chunk_added;
	uint32_t  chunk_hit;
};


int chunk_streamer_init(struct chunk_streamer *chunk_streamer,
		struct cache_manager *cache_manager);


int chunk_streamer_process_buffer(struct chunk_streamer *chunk_streamer,
		char *buffer, uint32_t buffer_size);

uint32_t chunk_streamer_flush_retained(struct chunk_streamer *chunk_streamer,
                char *buffer, uint32_t buffer_length);

#endif
