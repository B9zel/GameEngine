#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#pragma once
#include <Core/includes/PlatformDetection.h>
#include <Core/includes/Platform.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <memory>
#include <sstream>
#include <deque>
#include <stack>
#include <algorithm>



template<class TReturn, class... Args>
class Function;

#define STRINGCON_DETAILS(x) #x
#define STRINGCON(x) STRINGCON_DETAILS(x)


#define PASTE_DETAILS(x, y) x##y
#define PASTE(x, y) (PASTE_DETAILS(x, y))
#define FLAG_OFFSET(x) 1 << x


#define BIND_EVENT(Method, Class) Function<void(Event&)>(Method,Class)

//#define BIND_FUNCTION(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...);}

#define ENGINE_DEBUG _DEBUG
#define WITH_EDITOR 0

#ifdef ENGINE_DEBUG

#define DEVELOPMENT_DEBUG

#ifdef EG_PLATFORM_WINDOWS
#define ENGINE_DEBUGBREAK  __debugbreak()
#else
#define ENGINE_DEBUGBREAK 
#endif // _WIN64 || _WIN32

#define CORE_UNASSERT(is, log) { if (is) { EG_LOG(CORE, ELevelLog::CRITICAL, log); ENGINE_DEBUGBREAK; }}
#define CORE_ASSERT(is, log) { if (!(is)) { EG_LOG(CORE, ELevelLog::CRITICAL, log); ENGINE_DEBUGBREAK; }}
#define ASSERT(log) { EG_LOG(CORE, ELevelLog::CRITICAL, log); ENGINE_DEBUGBREAK; }
#define CHECK(is) { CORE_UNASSERT(!is, "Fail check"); }

#define FUNCTION_NAME __FUNCTION__
#define NUMBER_LINE  STRINGCON(__LINE__)

#define FUNCTION_STAT FUNCTION_NAME ":" NUMBER_LINE
#else
#define ENGINE_DEBUGBREAK 

#define CORE_CHECK(is) 
#define CHECK(is)
#define CORE_UNASSERT(is, log)
#define CORE_ASSERT(is, log)
#define ASSERT(log)
#define FUNCTION_NAME 
#define NUMBER_LINE

#define FUNCTION_STAT


#endif // ENGINE_DEBUG

using String = typename std::string;

using StringView = typename std::string_view;

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T, class Deleter = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, Deleter>;

template<class T>
using WeakPtr = std::weak_ptr<T>;

template<class T, class Allocattor = std::allocator<T>>
using DArray = std::vector<T, Allocattor>;

template<class T, size_t Size>
using StaticArray = std::array<T, Size>;

template<class First, class Second>
using Pair = std::pair<First, Second>;

template<class T>
using Deque = std::deque<T>;

template<class T, class Conteiner = Deque<T>>
using Stack = std::stack<T, Conteiner>;


template<class Key, class Value, class Hasher = std::hash<Key>>
using HashTableMap = std::unordered_map<Key, Value, Hasher>;

template<class Value, class Hasher = std::hash<Value>, class ValueEqual = std::equal_to<Value>>
using HashTableSet = std::unordered_set<Value, Hasher, ValueEqual>;

template<class T, class ... Args>
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

unsigned int AligneSizeofBy2(const unsigned int Bytes);


inline bool HasFlag(const uint32 FlagVar, const uint32 FindedFlag)
{
	return (FlagVar & FindedFlag) != 0;
}

inline void SetFlag(uint32& FlagVar, const uint32 AddFlag)
{
	FlagVar |= AddFlag;
}
inline void RemoveFlag(uint32& FlagVar, const uint32 RemFlag)
{
	FlagVar &= ~RemFlag;
}

#include <Templates/Function.h>
#include <Templates/Queue.h>
#include <Core/includes/Log.h>

