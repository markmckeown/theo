#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>

#define CHECKSUM_SIZE 32
#define CHECKSUM_INT_COUNT CHECKSUM_SIZE/8

struct checksum {
	uint8_t  bytes[CHECKSUM_SIZE];
};

void checksum_init(struct checksum *checksum);

int checksum_compare(struct checksum *first, struct checksum *second);

#endif
