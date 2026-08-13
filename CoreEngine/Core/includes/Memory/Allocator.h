#pragma once
#include <cstdlib>
#include <exception>
#include <memory>
#include <new>
#include <Core/includes/Platform.h>



class Allocator
{
public:

	/*
	*  Allocate memory
	*/
	static void* Allocate(size_t bytes);

	template<class T, class ...Args>
	static T* AllocateAndConstruct(size_t bytes, Args&& ...args);

	template<class T,class ...Args>
	static T* Allocate(Args&& ...args);

	static void* Reallocate(void* mem, size_t size);

	static void Deallocate(void* mem) noexcept;

	template<class T>
	static void Deallocate(T* mem);

	template<class T>
	static void DestroyAndDeallocate(T* mem) noexcept;

	template<class T> 
	static void Destruct(T* mem);

	template<class T, class ...Args>
	static void Construct(T* mem, Args&& ...args);
};


template<class T, class ...Args>
inline T* Allocator::AllocateAndConstruct(size_t bytes, Args&& ...args)
{
	if (bytes < sizeof(T))
	{
		throw std::bad_alloc();
	}

	void* rawMemory = Allocate(bytes);
	T* memory = static_cast<T*>(rawMemory);
	try
	{
		Construct(memory, std::forward<Args>(args)...);
	}
	catch (...)
	{
		Deallocate(rawMemory);
		throw;
	}

	return memory;
}

/*
*	Allocate memory and construct object
*/
template<class T, class ...Args>
inline T* Allocator::Allocate(Args&& ...args)
{
	return new T(std::forward<Args>(args)...);
}

template<class T>
inline void Allocator::Deallocate(T* mem)
{
	delete mem;
}

template<class T>
inline void Allocator::DestroyAndDeallocate(T* mem) noexcept
{
	if (!mem) return;

	mem->~T();
	Deallocate(static_cast<void*>(mem));
}
template <class T> inline void Allocator::Destruct(T* mem)
{
	if (!mem) return;

	mem->~T();
}

template<class T,class ...Args>
inline void Allocator::Construct(T* mem, Args&& ...args)
{
	if (!mem)
	{
		throw std::bad_alloc();
	}

	new(mem) T(std::forward<Args>(args)...);
}
