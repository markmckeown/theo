#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

#include "theo/checksum.h"

#define CHUNK_MAX_BUFFER_SIZE 32768

struct chunk {
	struct checksum checksum;
	char            buffer[CHUNK_MAX_BUFFER_SIZE];
	uint16_t        buffer_size;
};


void chunk_init(struct chunk *chunk);

void chunk_set(struct chunk *chunk, struct checksum *checksum, char *buffer,
		uint32_t buffer_size);

#endif
