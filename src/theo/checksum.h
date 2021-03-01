#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>

#define CHECKSUM_SIZE 32

struct checksum {
	uint8_t bytes[CHECKSUM_SIZE];
};

void checksum_init(struct checksum *checksum);

#endif
