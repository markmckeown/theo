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
#ifndef CACHE_HEADER_H
#define CACHE_HEADER_H

#define CACHE_HEADER_MAGIC 340865607434ull


#include <stdint.h>

struct cache_header {
	uint64_t cache_magic;
	uint64_t cell_count;
};

void cache_header_init(struct cache_header *cache_header, uint64_t cell_count);


#endif
