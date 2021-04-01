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

#include "theo/chunk_ref.h"
#include "theo/checksum.h"

void chunk_ref_init(struct chunk_ref *chunk_ref)
{
	memset(chunk_ref, 0, sizeof(struct chunk_ref));
	checksum_init(&chunk_ref->checksum);
	return;
}

void chunk_ref_set(struct chunk_ref *chunk_ref, char *buffer,
		   uint32_t buffer_size)
{
	checksum_gen(&chunk_ref->checksum, buffer, buffer_size);
	chunk_ref->buffer = buffer;
	chunk_ref->buffer_size = buffer_size;
}
