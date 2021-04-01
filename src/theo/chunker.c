/**
    Copyright 2021 WANdisco
    Author Mark Mc Keown

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

#include "theo/chunker.h"
#include "theo/chunk_ref.h"
#include "theo/msys.h"

int chunker_init(struct chunker *chunker)
{
	int ret = 0;
	memset(chunker, 0, sizeof(struct chunker));
	ret = rolling_hash2_init(&chunker->rh_state2, CHUNKER_WINDOW_WIDTH);
	ALWAYS(ret == 0);
	return ret;
}

int chunker_chunk_buffer(struct chunker *chunker,
			 struct chunk_ref *chunk_ref,
			 char *buffer, uint32_t buffer_length)
{
	int ret;
	int ret_t = 0;
	uint32_t offset;
	uint32_t max_len;
	ALWAYS(chunker->retained_size == 0);
	ALWAYS(buffer != 0);

	chunk_ref_init(chunk_ref);

	// If buffer is too small then store in chunker.
	if (buffer_length < CHUNKER_MIN_CHUNK_SIZE) {
		memcpy(chunker->retained, buffer, buffer_length);
		chunker->retained_size = buffer_length;
		ret = CHUNKER_NOT_FOUND;
		goto out;
	}

	rolling_hash2_reset(&chunker->rh_state2,
			    (uint8_t *) (buffer +
					 (CHUNKER_MIN_CHUNK_SIZE -
					  CHUNKER_WINDOW_WIDTH)));
	offset = CHUNKER_MIN_CHUNK_SIZE;
	if (buffer_length >= CHUNKER_MAX_CHUNK_SIZE) {
		max_len = CHUNKER_MAX_CHUNK_SIZE - CHUNKER_MIN_CHUNK_SIZE;
	} else {
		max_len = buffer_length - CHUNKER_MIN_CHUNK_SIZE;
	}
	ret_t =
	    rolling_hash2_run(&chunker->rh_state2, (uint8_t *) buffer, max_len,
			      CHUNKER_MASK, CHUNKER_TRIGGER, &offset);
	if (ret_t == FINGERPRINT_RET_HIT) {
		chunk_ref_set(chunk_ref, buffer,
			      CHUNKER_MIN_CHUNK_SIZE + offset);
		ret = CHUNKER_FOUND;
		goto out;
	}
	// Hit FINGERPRINT_RET_MAX and CHUNKER_MAX_CHUNK_SIZE
	if (max_len == CHUNKER_MAX_CHUNK_SIZE - CHUNKER_MIN_CHUNK_SIZE) {
		chunk_ref_set(chunk_ref, buffer, CHUNKER_MAX_CHUNK_SIZE);
		ret = CHUNKER_FOUND;
		goto out;
	}
	// Hit FINGERPRINT_RET_MAX but less than CHUNKER_MAX_CHUNK_SIZE, stash buffer.
	memcpy(chunker->retained, buffer, buffer_length);
	chunker->retained_size = buffer_length;
	ret = CHUNKER_NOT_FOUND;

out:
	return ret;
}

uint32_t chunker_flush_retained(struct chunker *chunker,
				char *buffer,
				__attribute__((unused)) uint32_t buffer_length)
{
	ALWAYS(buffer_length > chunker->retained_size);
	uint32_t ret = 0;

	if (chunker->retained_size == 0) {
		goto out;
	}
	ret = chunker->retained_size;
	memcpy(buffer, chunker->retained, chunker->retained_size);
	chunker->retained_size = 0;

out:
	return ret;
}
