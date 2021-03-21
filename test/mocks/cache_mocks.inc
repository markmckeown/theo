#ifdef MOCK_CACHE_INIT
int cache_init(struct cache *cache, char *buffer, uint64_t length) 
{
	int res=  (int) mock(cache, buffer, length);
	return res;
}
#undef MOCK_CACHE_INIT
#endif

