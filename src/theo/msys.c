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
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>


int msys_access(const char *pathname, int mode) {
	int ret = access(pathname, mode);
	return ret;
}

int msys_unlink(const char *pathname) {
	int ret = remove(pathname);
	return ret;
}


int msys_open(const char *pathname, int flags) {
	int ret = open(pathname, flags, S_IRUSR | S_IWUSR);
	return ret;
}

int msys_stat(const char *path, struct stat *buf) {
	int ret = stat(path, buf);
	return ret;
}

ssize_t msys_write(int fd, const void *buf, size_t count) {
	ssize_t ret = write(fd, buf, count);
	return ret;
}

int msys_close(int fd) {
	int ret = close(fd);
	return ret;
}

int msys_fsync(int fd) {
	int ret = fsync(fd);
	return ret;
}

void *msys_mmap(void *addr, size_t length, int prot, int flags, int fd,
                off_t offset) {
	void *ret = mmap(addr, length, prot, flags, fd, offset);
	return ret;
}

int msys_munmap(void *addr, size_t length) {
	int ret = munmap(addr, length);
	return ret;
}
