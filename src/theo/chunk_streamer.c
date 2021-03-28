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


#include <string.h>

#include "theo/chunk_streamer.h"
#include "theo/chunk_ref.h"
#include "theo/cache_manager.h"

int chunk_streamer_init(struct chunk_streamer *chunk_streamer,
		struct cache_manager *cache_manager) {
	int ret = 0;
	memset(chunk_streamer, 0, sizeof(struct chunk_streamer));
	ret = chunker_init(&chunk_streamer->chunker);
	chunk_streamer->cache_manager = cache_manager;
	return ret;
}

int chunk_streamer_process_buffer(struct chunk_streamer *chunk_streamer,
		char *buffer, uint32_t buffer_size) {
	int ret = 0;
	int ret_t = 0;
	uint32_t buffer_left;
	char *ptr;
	struct chunk_ref chunk_ref;

	ptr = buffer;
	buffer_left = buffer_size;
	for (;;) {
		chunk_ref_init(&chunk_ref);
		ret_t = chunker_chunk_buffer(&chunk_streamer->chunker, 
			       &chunk_ref, ptr, buffer_left);
		if (ret_t == CHUNKER_NOT_FOUND) {
			// Buffer fully porcessed.
			break;
		}

		if (cache_manager_has_chunk(chunk_streamer->cache_manager,
						&chunk_ref.checksum)) {
			chunk_streamer->chunk_hit++;
		} else {
			cache_manager_add_chunk(chunk_streamer->cache_manager, 
				&chunk_ref.checksum, chunk_ref.buffer, 
				chunk_ref.buffer_size);
			chunk_streamer->chunk_added++;
		}
		ptr += chunk_ref.buffer_size;
		buffer_left -= chunk_ref.buffer_size;
	}

	return ret;
}


uint32_t chunk_streamer_flush_retained(struct chunk_streamer *chunk_streamer,
                char *buffer, uint32_t buffer_length) {
	uint32_t ret;
	ret = chunker_flush_retained(&chunk_streamer->chunker, 
			buffer, buffer_length);
	return ret;
}
