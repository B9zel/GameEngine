#pragma once

#include <Core/includes/PlatformDetection.h>
#include <string>
#include <vector>
//#include <functional>
#include <unordered_map>
#include <array>
#include <memory>
#include <sstream>



template<class TReturn, class... Args>
class Function;
	
#define STRINGCON_DETAILS(x) #x
#define STRINGCON(x) STRINGCON_DETAILS(x)


#define PASTE_DETAILS(x, y) x##y
#define PASTE(x, y) (PASTE_DETAILS(x, y))


#define BIND_EVENT(Method, Class) Function<void(Event&)>(Method,Class)

//#define BIND_FUNCTION(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...);}



#ifdef _DEBUG

#define DEVELOPMENT_DEBUG

#ifdef EG_PLATFORM_WINDOWS
	#define ENGINE_DEBUGBREAK  __debugbreak()
#else
	#define ENGINE_DEBUGBREAK 
#endif // _WIN64 || _WIN32

	#define CORE_ASSERT(is, log) { if (is) { EG_LOG(CORE, CoreEngine::ELevelLog::CRITICAL, log); ENGINE_DEBUGBREAK; }}
	
	#define FUNCTION_NAME __FUNCTION__
	#define NUMBER_LINE  STRINGCON(__LINE__)

	#define FUNCTION_STAT FUNCTION_NAME ":" NUMBER_LINE
#else
	#define ENGINE_DEBUGBREAK 

	#define CORE_CHECK(is) 
	#define CORE_ASSERT(is, log)
	#define FUNCTION_NAME 
	#define NUMBER_LINE

	#define FUNCTION_STAT

	
#endif // _DEBUG

using String = typename std::string;

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T>
using UniquePtr = std::unique_ptr<T>;

template<class T,class Allocattor=std::allocator<T>>
using DArray = std::vector<T, Allocattor>;

template<class T, size_t Size>
using StaticArray = std::array<T, Size>;

//template<class T>
//using Function = std::function<T>;



template<class Key,class Value, class Hasher = std::hash<Key>>
using HashTableMap = std::unordered_map<Key, Value, Hasher>;


template<class T,class ... Args>
UniquePtr<T> MakeUniquePtr(Args&& ...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<class T, class ... Args>
SharedPtr<T> MakeSharedPtr(Args&& ...args)
{
	return std::make_shared <T>(std::forward<Args>(args)...);
}

template<class T>
String ToString(T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template<class T>
T FromString(const String& str)
{
	std::istringstream iss(str);
	T toValue;
	iss >> toValue;
	return toValue;
}


#include <Core/includes/Log.h>
#include <Templates/Function.h>

