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
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

#include "theo/xmalloc.h"
#include "theo/cache_manager.h"
#include "theo/msys.h"

#define CACHE_MANAGER_MB 1048576ull

void cache_manager_init(struct cache_manager *cache_manager)
{
	memset(cache_manager, 0, sizeof(struct cache_manager));
	cache_manager->fd = -1;
	return;
}

uint64_t cache_manager_filesize(uint64_t requested_size)
{
	uint64_t mb = 0;

	mb = requested_size / CACHE_MANAGER_MB;
	mb++;
	return mb * CACHE_MANAGER_MB;
}

int cache_manager_create_file(char *filename, uint64_t size)
{
	int ret_t = 0;
	int ret = 0;
	int fd;
	char *buffer;
	uint64_t mb = 0;
	uint64_t i = 0;
	uint64_t written = 0;

	mb = size / CACHE_MANAGER_MB;
	fd = msys_open(filename, O_RDWR | O_CREAT);
	if (fd < 0) {
		ret = -1;
		goto out;
	}

	buffer = xmalloc(CACHE_MANAGER_MB);
	memset(buffer, 0, CACHE_MANAGER_MB);

	for (i = 0; i < mb; i++) {
		written = 0;
		do {
			ret_t = msys_write(fd, buffer + written,
					   CACHE_MANAGER_MB - written);
			if (ret_t < 0) {
				ret = -1;
				goto err;
			}
			written += ret_t;
		} while (written < CACHE_MANAGER_MB);
	}
	ret_t = msys_fsync(fd);
	ALWAYS(ret_t == 0);

err:
	ret_t = msys_close(fd);
	ALWAYS(ret_t == 0);
	xfree(buffer);
out:
	return ret;
}

int cache_manager_check_file(char *filename, int64_t required_size)
{
	int ret = 0;
	int ret_t = 0;
	struct stat statbuf;

	ret_t = msys_stat(filename, &statbuf);
	if (ret_t != 0) {
		// file does not exist
		ret_t = cache_manager_create_file(filename, required_size);
		if (ret_t != 0) {
			ret = ret_t;
			goto out;
		}
	} else if (statbuf.st_size != required_size) {
		// file exists but is the wrong size.
		ret_t = msys_unlink(filename);
		if (ret_t != 0) {
			ret = ret_t;
			goto out;
		}
		ret_t = cache_manager_create_file(filename, required_size);
		if (ret_t != 0) {
			ret = ret_t;
			goto out;
		}
	}

out:
	return ret;
}

int cache_manager_open_cache(struct cache_manager *cache_manager,
			     char *filename, uint64_t cache_size)
{
	int ret = 0;
	int ret_t = 0;
	char *ptr = 0;

	cache_manager->memory_size = cache_manager_filesize(cache_size);

	ret_t = cache_manager_check_file(filename, cache_manager->memory_size);
	if (ret_t != 0) {
		ret = ret_t;
		goto out;
	}

	cache_manager->fd = msys_open(filename, O_RDWR);
	if (cache_manager->fd < 0) {
		ret = cache_manager->fd;
		goto out;
	}

	ptr = msys_mmap(NULL, cache_manager->memory_size,
			PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE | MAP_SYNC,
			cache_manager->fd, 0);
	if (ptr == MAP_FAILED) {
		if (errno == EOPNOTSUPP) {
			// No DAX support try normal mmap
			ptr = msys_mmap(NULL, cache_manager->memory_size,
					PROT_READ | PROT_WRITE,
					MAP_SHARED_VALIDATE, cache_manager->fd,
					0);
			if (ptr == MAP_FAILED) {
				ret = -1;
				goto err;
			}
		} else {
			ret = -1;
			goto err;
		}
	} else {
		cache_manager->dax = 1;
	}
	cache_manager->ptr = ptr;

	cache_init(&cache_manager->cache, cache_manager->ptr,
		   cache_manager->memory_size);
out:
	return ret;

err:
	cache_manager_release(cache_manager);
	return ret;
}

void cache_manager_release(struct cache_manager *cache_manager)
{
	__attribute__((unused)) int ret = 0;
	cache_release(&cache_manager->cache);

	if (cache_manager->ptr != 0) {
		ret = msys_munmap(cache_manager->ptr,
				  cache_manager->memory_size);
		ALWAYS(ret == 0);
		cache_manager->ptr = 0;
	}

	if (cache_manager->fd != -1) {
		ret = msys_close(cache_manager->fd);
		ALWAYS(ret == 0);
		cache_manager->fd = -1;
	}

	return;
}

bool cache_manager_add_chunk(struct cache_manager *cache_manager,
			     struct checksum *checksum,
			     char *buffer, uint32_t buffer_size)
{
	return cache_add_chunk(&cache_manager->cache, checksum, buffer,
			       buffer_size);
}

bool cache_manager_has_chunk(struct cache_manager *cache_manager,
			     struct checksum *checksum)
{
	return cache_has_chunk(&cache_manager->cache, checksum);
}

bool cache_manager_get_chunk(struct cache_manager *cache_manager,
			     struct checksum *checksum, struct chunk *chunk)
{
	return cache_get_chunk(&cache_manager->cache, checksum, chunk);
}

void cache_manager_cache_metrics(struct cache_manager *cache_manager,
				 struct cache_metrics *cache_metrics)
{
	return cache_get_metrics(&cache_manager->cache, cache_metrics);
}
