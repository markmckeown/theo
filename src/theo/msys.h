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
#ifndef MSYS_H_
#define MSYS_H_

#include <assert.h>
#include <sys/types.h>

#if defined(COVERAGE)
# define ALWAYS(X)
# define NEVER(X)
#else
# define ALWAYS(X)      assert(X)
# define NEVER(X)       assert(!(X))
#endif

// Forward declarations.
struct stat;

int msys_access(const char *pathname, int mode);

int msys_unlink(const char *pathname);


int msys_open(const char *pathname, int flags);


int msys_stat(const char *path, struct stat *buf);


ssize_t msys_write(int fd, const void *buf, size_t count);


int msys_close(int fd);

int msys_fsync(int fd);


void *msys_mmap(void *addr, size_t length, int prot, int flags, int fd,
                off_t offset);


int msys_munmap(void *addr, size_t length);


#endif /* MSYS_H_ */
