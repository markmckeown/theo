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
#include <string.h>

#include "theo/cache_metrics.h"

void cache_metrics_init(struct cache_metrics *cache_metrics)
{
	memset(cache_metrics, 0, sizeof(struct cache_metrics));
	return;
}
