#include "../CoreEngine/Core/includes/Memory/Allocator.h"

inline void* Allocator::Allocate(unsigned int bytes) noexcept
{
    return std::malloc(bytes);
}

inline void* Allocator::Reallocate(void* mem, unsigned int size) noexcept
{
    return std::realloc(mem, size);
}

inline void Allocator::Deallocate(void* mem) noexcept
{
    return std::free(mem);
}

