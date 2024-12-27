#include <cstring>
#include <cstdlib>

// Force heap allocations on Mac to also memset to 0xcd
// So that uninitialized variables behave the same on Mac/PC
#if defined(__APPLE__)
void* operator new(size_t count)
{
	void* ptr = std::malloc(count);
	std::memset(ptr, 0xcd, count);
	return ptr;
}

void* operator new[](size_t count)
{
	void* ptr = std::malloc(count);
	std::memset(ptr, 0xcd, count);
	return ptr;
}

void operator delete(void* ptr) noexcept
{
	std::free(ptr);
}

void operator delete[](void* ptr) noexcept
{
	std::free(ptr);
}
#endif
