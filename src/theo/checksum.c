#include <string.h>

#include "theo/checksum.h"


void checksum_init(struct checksum *checksum) {
	memset(checksum, 0, sizeof(struct checksum));
	return;
}

int checksum_compare(struct checksum *first, struct checksum *second) {
	return memcmp(first, second, sizeof(struct checksum));
}
