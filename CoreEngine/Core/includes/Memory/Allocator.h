#pragma once
#include <cstdlib>
#include <exception>
#include <memory>
#include <Core/includes/Platform.h>



class Allocator
{
public:

	/*
	*  Allocate memory
	*/
	static void* Allocate(uint32 bytes) noexcept;

	template<class T, class ...Args>
	static T* AllocateAndConstruct(uint32 bytes, Args&& ...args);

	template<class T,class ...Args>
	static T* Allocate(Args&& ...args);

	static void* Reallocate(void* mem, uint32 size) noexcept;

	template<class T, class ...Args>
	static T* Reallocate(void* mem, uint32 size, Args&& ...args);

	static void Deallocate(void* mem) noexcept;

	template<class T>
	static void Deallocate(T* mem);

	template<class T, class ...Args>
	static void Construct(T* mem, Args&& ...args);
};


template<class T, class ...Args>
inline T* Allocator::AllocateAndConstruct(uint32 bytes, Args&& ...args)
{
	void* Mem = Allocate(bytes);
	Construct(Mem, std::move(args));
	return Mem;
}

/*
*	Allocate memory and construct object
*/
template<class T, class ...Args>
inline T* Allocator::Allocate(Args&& ...args)
{
	return new T(std::forward<Args>(args)...);
}

template<class T, class ...Args>
inline T* Allocator::Reallocate(void* mem, uint32 size, Args && ...args)
{
	if (sizeof(T) < size)
	{
		throw std::bad_alloc();
	}

	void* pMem = realloc(mem, size);
	::new(pMem) T(std::forward<Args>(args)...);

	return (T*)pMen;
}

template<class T>
inline void Allocator::Deallocate(T* mem)
{
	mem->~T();
	std::free(mem);
}

template<class T,class ...Args>
inline void Allocator::Construct(T* mem, Args&& ...args)
{
	new(mem) T(std::forward<Args>(args)...);
}
