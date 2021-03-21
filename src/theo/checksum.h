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
