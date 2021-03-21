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

#include "theo/cache.h"
#include "theo/cell.h"
#include "theo/chunk.h"
#include "theo/checksum.h"
#include "theo/cache_header.h"
#include "theo/cache_metrics.h"
#include "theo/msys.h"
#include "theo/xmalloc.h"



int cache_init(struct cache *cache,
                char         *cache_buffer,
                uint64_t      cache_buffer_size) {
	uint64_t slots = 0;
	uint64_t cell_count = 0;
	uint64_t i = 0;
	uint32_t cell_size = CACHE_CELL_SIZE;
	char *ptr;
	struct cell *cell;

	memset(cache, 0, sizeof(struct cache));
	slots = cache_buffer_size/cell_size;
	ALWAYS(slots >= 2);

	cell_count = slots - 1;
	ptr = cache_buffer;
	cache->cache_header = (struct cache_header *) ptr;
	cache_header_init(cache->cache_header, cell_count);

	ptr += CACHE_CELL_SIZE;
	cache->cells = xmalloc(cell_count * sizeof(struct cell));

	cell = cache->cells;
	for (i = 0; i < cell_count; i++) {
		cell_init(cell, ptr, cell_size);
		cell++;
		ptr += CACHE_CELL_SIZE;
	}	

	return 0;
}

struct cell* cache_get_cell_index(struct cache *cache,struct checksum *checksum) {
	uint64_t checksum_part = 0;
	uint64_t index = 0;
	struct cell *cell;

	memcpy(&checksum_part, checksum->bytes, sizeof(uint64_t));
	index = checksum_part % cache->cache_header->cell_count;
	cell = cache->cells;
	cell += index;

	return cell;
}

bool cache_add_chunk(struct cache *cache, struct checksum *checksum,
		char *buffer, uint32_t buffer_size) {
	struct cell *cell;

	cell = cache_get_cell_index(cache, checksum);
	return cell_add_chunk(cell, checksum, buffer, buffer_size);
}

bool cache_has_chunk(struct cache *cache, struct checksum *checksum) {
	struct cell *cell;

	cell = cache_get_cell_index(cache, checksum);
	return cell_has_chunk(cell, checksum);
}

bool cache_get_chunk(struct cache *cache, struct checksum *checksum,
                struct chunk *chunk) {
	struct cell *cell;

	cell = cache_get_cell_index(cache, checksum);
	return cell_get_chunk(cell, checksum, chunk);
}

void cache_release(struct cache *cache) {
	struct cell *cell;	
	uint64_t i = 0;

	if (cache->cells == 0) {
		goto out;
	}

	cell = cache->cells;
	for (i = 0; i < cache->cache_header->cell_count; i++) {
		cell_release(cell);
		cell++;
	}	

	xfree(cache->cells);
	cache->cells = 0;
out:
	return;
}


void cache_get_metrics(struct cache *cache, 
		struct cache_metrics *cache_metrics) {
	struct cell *cell;	
	uint64_t i = 0;

	cache_metrics_init(cache_metrics);
	cell = cache->cells;
	for (i = 0; i < cache->cache_header->cell_count; i++) {
		cell_get_metrics(cell, cache_metrics);
		cell++;
	}	

	return;
}
