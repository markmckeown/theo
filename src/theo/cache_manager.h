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
#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#include "theo/cache.h"

struct cache_metrics;

struct cache_manager {
	struct cache           cache;
	char                  *ptr;
	int64_t                memory_size;
	int                    fd;
	int                    dax;
};

void cache_manager_init(struct cache_manager *cache_manager);

int cache_manager_open_cache(struct cache_manager *cache_manager,
		char* filename,
		uint64_t cache_size);

void cache_manager_release(struct cache_manager *cache_manager);

bool cache_manager_add_chunk(struct cache_manager *cache_managrt, 
		struct checksum *checksum,
		char *buffer, uint32_t buffer_size);

bool cache_manager_has_chunk(struct cache_manager *cache_manager, 
		struct checksum *checksum);

bool cache_manager_get_chunk(struct cache_manager *cache_manager, 
		struct checksum *checksum, 
		struct chunk *chunk);

void cache_manager_cache_metrics(struct cache_manager *cache_manager,
		struct cache_metrics *cache_metrics);

int cache_manager_create_file(char *filename, uint64_t size);


int cache_manager_check_file(char *filename, int64_t required_size);


int cache_manager_open_cache(struct cache_manager *cache_manager,
		char* filename,
		uint64_t cache_size);


#endif
