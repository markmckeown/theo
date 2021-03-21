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

#include "theo/cell.h"
#include "theo/chunk.h"
#include "theo/cell_header.h"
#include "theo/cache_metrics.h"
#include "theo/cell_body_entry.h"
#include "theo/msys.h"

void cell_init(struct cell *cell, char *slab, uint32_t slab_size) {
	__attribute__((unused)) int ret_t = 0;

	ALWAYS(slab_size > sizeof(struct cell_header));
	memset(cell, 0, sizeof(struct cell));
	cell->slab = slab;
	cell->slab_size = slab_size;
	cell->cell_header = (struct cell_header *) (slab + (slab_size - sizeof(struct cell_header)));
	if (!cell_header_sane(cell->cell_header)) {
		cell_header_init(cell->cell_header, cell);
	}
	ret_t = pthread_mutex_init(&cell->mutex, NULL);
	ALWAYS(ret_t == 0);
	return;
}

void cell_release(struct cell *cell) {
	__attribute__((unused)) int ret_t = 0;
	ret_t = pthread_mutex_destroy(&cell->mutex);
	ALWAYS(ret_t == 0);
	return;
}

bool cell_add_chunk(struct cell *cell, 
		   struct checksum *checksum, 
		   char *buffer, 
		   uint32_t buffer_size) {
	bool ret = true;
	__attribute__((unused)) int ret_t = 0;
	struct cell_dir_entry cell_dir_entry;
	
	cell_dir_entry_init(&cell_dir_entry);
	
	ret_t = pthread_mutex_lock(&cell->mutex);
	ALWAYS(ret_t == 0);

	// Is the buffer already added?
	if (cell_dir_get_chunk(&cell->cell_header->cell_dir, 
			cell->slab + (cell->slab_size - sizeof(struct cell_header)),
			checksum,
			&cell_dir_entry)) {
		ret = false;
		goto out;
	}

	cell_ic_on_entry(&cell->cell_header->cell_ic);

	cell_body_add(&cell->cell_header->cell_body, cell,
			checksum, buffer, buffer_size, &cell_dir_entry);

	cell_ic_on_exit(&cell->cell_header->cell_ic);

out:
	ret_t = pthread_mutex_unlock(&cell->mutex);
	ALWAYS(ret_t == 0);
	return ret;
}




bool cell_get_chunk(struct cell *cell,
		struct checksum *checksum,
		struct chunk *chunk) {
	bool ret = true;
	__attribute__((unused)) int ret_t = 0;
	struct cell_dir_entry cell_dir_entry;
	struct cell_body_entry *cell_body_entry;
	char *ptr;

	ret_t = pthread_mutex_lock(&cell->mutex);
	ALWAYS(ret_t == 0);

	cell_dir_entry_init(&cell_dir_entry);
	if (!cell_dir_get_chunk(&cell->cell_header->cell_dir, 
			cell->slab + (cell->slab_size - sizeof(struct cell_header)),
			checksum,
			&cell_dir_entry)) {
		ret = false;
		goto out;
	}

	ptr = cell->slab + cell_dir_entry.offset;
	cell_body_entry = (struct cell_body_entry *) ptr;
	ptr += sizeof(struct cell_body_entry);
	chunk_set(chunk, &cell_dir_entry.checksum, ptr, cell_body_entry->size);

out:	
	ret_t = pthread_mutex_unlock(&cell->mutex);
	ALWAYS(ret_t == 0);
	return ret;
}

bool cell_has_chunk(struct cell *cell,
		struct checksum *checksum) {
	bool ret;
	__attribute__((unused)) int ret_t = 0;
	struct cell_dir_entry cell_dir_entry;

	ret_t = pthread_mutex_lock(&cell->mutex);
	ALWAYS(ret_t == 0);

	cell_dir_entry_init(&cell_dir_entry);
	ret = cell_dir_get_chunk(&cell->cell_header->cell_dir, 
			cell->slab + (cell->slab_size - sizeof(struct cell_header)),
			checksum,
			&cell_dir_entry);

	ret_t = pthread_mutex_unlock(&cell->mutex);
	ALWAYS(ret_t == 0);
	return ret;
}


int32_t cell_entry_count(struct cell *cell) {
	int32_t ret = 0;

	__attribute__((unused)) int ret_t = 0;
	ret_t = pthread_mutex_lock(&cell->mutex);
	ALWAYS(ret_t == 0);

	ret = cell_header_entry_count(cell->cell_header);

	ret_t = pthread_mutex_unlock(&cell->mutex);
	ALWAYS(ret_t == 0);

	return ret;
}

int32_t cell_byte_count(struct cell *cell) {
	int32_t ret = 0;

	__attribute__((unused)) int ret_t = 0;
	ret_t = pthread_mutex_lock(&cell->mutex);
	ALWAYS(ret_t == 0);

	ret = cell_header_byte_count(cell->cell_header);

	ret_t = pthread_mutex_unlock(&cell->mutex);
	ALWAYS(ret_t == 0);

	return ret;
}

void cell_get_metrics(struct cell *cell, struct cache_metrics *cache_metrics) {
	__attribute__((unused)) int ret_t = 0;
	ret_t = pthread_mutex_lock(&cell->mutex);
	ALWAYS(ret_t == 0);

	cache_metrics->byte_count +=  cell_header_byte_count(cell->cell_header);
	cache_metrics->entry_count += cell_header_entry_count(cell->cell_header);

	ret_t = pthread_mutex_unlock(&cell->mutex);
	ALWAYS(ret_t == 0);

	return;
}
