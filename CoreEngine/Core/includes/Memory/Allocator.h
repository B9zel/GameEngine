#pragma once
#include <cstdlib>
#include <exception>
#include <memory>




class Allocator
{
public:

	static void* Allocate(unsigned int bytes) noexcept;

	template<class T,class ...Args>
	static T* Allocate(Args&& ...args);

	static void* Reallocate(void* mem, unsigned int size) noexcept;

	template<class T, class ...Args>
	static T* Reallocate(void* mem, unsigned int size, Args&& ...args);

	static void Deallocate(void* mem) noexcept;

	template<class T>
	static void Deallocate(T* mem);

	template<class T, class ...Args>
	static void Construct(T* mem, Args&& ...args);
};


template<class T, class ...Args>
inline T* Allocator::Allocate(Args&& ...args)
{
	return new T(std::forward<Args>(args)...);
}

template<class T, class ...Args>
inline T* Allocator::Reallocate(void* mem, unsigned int size, Args && ...args)
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
	if (std::is_class_v<T>)
	{
		mem->~T();
	}
	std::free(mem);
}

template<class T,class ...Args>
inline void Allocator::Construct(T* mem, Args&& ...args)
{
	new(mem) T(std::forward<Args>(args)...);
	//new((T*)mem) T(std::move(args));
}
