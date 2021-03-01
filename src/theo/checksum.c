#include <string.h>

#include "theo/checksum.h"


void checksum_init(struct checksum *checksum) {
	memset(checksum, 0, sizeof(struct checksum));
	return;
}
