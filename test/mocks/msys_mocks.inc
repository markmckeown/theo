
#ifdef MOCK_MSYS_OPEN
int msys_open(const char *pathname, int flags)
{
	if (g_start_mocking==1) {
		int ret= (int) mock(pathname, flags);
                return ret;
	} else {
		return open(pathname,flags);
	}
}
#undef MOCK_MSYS_OPEN
#endif


#ifdef MOCK_MSYS_WRITE
ssize_t msys_write(int fd, const void *buf, size_t count)
{
	if (g_start_mocking==1) {
		ssize_t ret= (ssize_t) mock(fd, buf, count);
		return ret;
	} else {
		return write(fd, buf, count);
	}
}
#undef MOCK_MSYS_WRITE
#endif


#ifdef MOCK_MSYS_CLOSE
int msys_close(int fd)
{
	if (g_start_mocking==1) {
		int ret= (int) mock(fd);;
		return ret;
	} else {
		return close(fd);
	}
}
#undef MOCK_MSYS_CLOSE
#endif

#ifdef MOCK_MSYS_FSYNC
int msys_fsync(int fd)
{
	if (g_start_mocking==1) {
		int ret= (int) mock(fd);;
		return ret;
	} else {
		return fsync(fd);
	}
}
#undef MOCK_MSYS_FSYNC
#endif


#ifdef MOCK_MSYS_STAT
// This global value allows us to control what is returned
// by stat
int stat_buf_st_size;
int msys_stat(const char *path, struct stat *buf)
{
	if (g_start_mocking==1) {
		buf->st_size = stat_buf_st_size;
		int ret=   (int) mock(path, buf);
		return ret;
	} else {
		return stat(path, buf);
	}
}
#undef MOCK_MSYS_STAT
#endif

#ifdef MOCK_MSYS_UNLINK
int msys_unlink(const char *pathname)
{
	if (g_start_mocking==1) {
		int ret = (int) mock(pathname);
		return ret;
	} else {
		return unlink(pathname);
	}
}
#undef MOCK_MSYS_UNLINK
#endif

#ifdef MOCK_MSYS_MMAP
void* msys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	if (g_start_mocking==1) {
		void* ret = (void*) mock(addr,  length,  prot,  flags,  fd,  offset);
		return ret;
	} else {
		return mmap(addr,  length,  prot,  flags,  fd,  offset);
	}
}
#undef MOCK_MSYS_MMAP
#endif

#ifdef MOCK_MSYS_MUNMAP
int msys_munmap(void *addr, size_t length)
{
	if (g_start_mocking==1) {
		int res=  (int) mock(addr, length);
		return res;
	}else{
		return munmap(addr, length);
	}
}
#undef MOCK_MSYS_MUNMAP
#endif
