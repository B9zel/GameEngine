#include <Core/includes/Memory/Allocator.h>

void* Allocator::Allocate(size_t bytes)
{
	if (void* memory = std::malloc(bytes))
	{
		return memory;
	}

	throw std::bad_alloc();
}

void* Allocator::Reallocate(void* mem, size_t size)
{
	if (size == 0)
	{
		Deallocate(mem);
		return nullptr;
	}

	if (void* memory = std::realloc(mem, size))
	{
		return memory;
	}

	throw std::bad_alloc();
}

void Allocator::Deallocate(void* mem) noexcept
{
	std::free(mem);
}

