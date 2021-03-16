#include "theo/cache_header.h"


void cache_header_init(struct cache_header *cache_header, uint64_t cell_count) {
	if (cache_header->cache_magic == CACHE_HEADER_MAGIC) {
		goto out;
	}

	cache_header->cell_count = cell_count;
	// flush cell count;
	__sync_synchronize();

	cache_header->cache_magic = CACHE_HEADER_MAGIC;
	// flush magic
	__sync_synchronize();
out:
	return;
}
