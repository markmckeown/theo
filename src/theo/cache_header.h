#ifndef CACHE_HEADER_H
#define CACHE_HEADER_H

#define CACHE_HEADER_MAGIC 340865607434ull


#include <stdint.h>

struct cache_header {
	uint64_t cache_magic;
	uint64_t cell_count;
};

void cache_header_init(struct cache_header *cache_header, uint64_t cell_count);


#endif
