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

#include "isa-l_crypto.h"

#include "theo/checksum.h"

void checksum_init(struct checksum *checksum)
{
	memset(checksum, 0, sizeof(struct checksum));
	return;
}

int checksum_compare(struct checksum *first, struct checksum *second)
{
	return memcmp(first, second, sizeof(struct checksum));
}

void checksum_gen(struct checksum *checksum, char *buffer,
		  uint32_t buffer_length)
{
	struct mh_sha256_ctx ctx;

	mh_sha256_init(&ctx);
	mh_sha256_update(&ctx, buffer, buffer_length);
	mh_sha256_finalize(&ctx, (uint32_t *) & checksum->bytes);
	return;
}
