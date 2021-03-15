#include <string.h>

#include "theo/chunk.h"
#include "theo/msys.h"


void chunk_init(struct chunk *chunk) {
	memset(chunk, 0, sizeof(struct chunk));
	checksum_init(&chunk->checksum);
	return;
}

void chunk_set(struct chunk *chunk, struct checksum *checksum, char *buffer,
		uint32_t buffer_size) {
	ALWAYS(buffer_size <= CHUNK_MAX_BUFFER_SIZE);
	memcpy(&chunk->checksum, checksum, sizeof(struct checksum));
	memcpy(chunk->buffer, buffer, buffer_size);
	chunk->buffer_size = buffer_size;
	return;
}
