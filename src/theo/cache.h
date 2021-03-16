#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <stdbool.h>

#define CACHE_CELL_SIZE 1024*1024ul

// Forward declaration.
struct cell;
struct cache_header;
struct checksum;
struct chunk;

struct cache {
	struct cell           *cells;
	struct cache_header   *cache_header;
	char                  *cache_buffer;
	uint64_t               cache_buffer_size;
};

void cache_init(struct cache *cache,
		char         *cache_buffer,
		uint64_t      cache_buffer_size);

void cache_add_chunk(struct cache *cache, struct checksum *checksum,
		char *buffer, uint32_t buffer_size);

bool cache_has_chunk(struct cache *cache, struct checksum *checksum);

bool cache_get_chunk(struct cache *cache, struct checksum *checksum, 
		struct chunk *chunk);


void cache_release(struct cache *cache);

#endif
