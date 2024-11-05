#include "../CoreEngine/Core/includes/Memory/Allocator.h"

void* Allocator::Allocate(uint32 bytes) noexcept
{
    return std::malloc(bytes);
}

inline void* Allocator::Reallocate(void* mem, uint32 size) noexcept
{
    return std::realloc(mem, size);
}

void Allocator::Deallocate(void* mem) noexcept
{
    return std::free(mem);
}

