#pragma once
#include <spdlog/spdlog.h>
#include <Core/includes/Base.h>


class Log;
enum class ELevelLog;
class LogCategory;

#ifdef ENGINE_DEBUG

#define EG_LOG_INFO(...) Log::GetLog()->info(__VA_ARGS__)
#define EG_LOG_ERROR(...) Log::GetLog()->error(__VA_ARGS__)
#define EG_LOG_CRITICAL(...) Log::GetLog()->critical(__VA_ARGS__)
#define EG_LOG_WARN(...) Log::GetLog()->warn(__VA_ARGS__)
																		
#define DECLARE_LOG_CATEGORY_EXTERN(Category) \
		static class LogCategory##Category : public LogCategory \
		{														\
		public:													\
			LogCategory##Category() : LogCategory(#Category) {} \
		} Category;

#define EG_LOG(Category,levelLog, ...) \
	{	\
		Log::LogOutput(Category, levelLog, FUNCTION_STAT, __VA_ARGS__); \
	}

#else
#define EG_LOG_INFO(...) 
#define EG_LOG_ERROR(...)
#define EG_LOG_CRITICAL(...) 
#define EG_LOG_WARN(...) 

#define DECLARE_LOG_CATEGORY_EXTERN(Category)
#define EG_LOG(Category,levelLog, ...)
#endif // ENGINE_DEBUG

	

enum class ELevelLog : int32
{
	INFO,
	ERROR,
	CRITICAL,
	WARNING
};

	
class LogCategory
{
public:
	LogCategory(String name) : m_Name{name} {}
	
	String GetName() const { return m_Name;  }
private:
	String m_Name;
};

	
namespace CoreEngine
{
	DECLARE_LOG_CATEGORY_EXTERN(CORE)
}

class Log
{
public:

	static void Init();

	static SharedPtr<spdlog::logger>& GetLog() { return m_Log; }

	template<class T>
	static void LogOutput(const LogCategory& categoryName, const ELevelLog levelLog, const char* function,const T& text)
	{
		switch (levelLog)
		{
		case ELevelLog::INFO:
			EG_LOG_INFO("[{0} {1}]: {2}", categoryName.GetName(), function, text);
			break;
		case ELevelLog::ERROR:
			EG_LOG_ERROR("[{0} {1}]: {2}", categoryName.GetName(), function, text);
			break;
		case ELevelLog::CRITICAL:
			EG_LOG_CRITICAL("[{0} {1}]: {2}", categoryName.GetName(), function, text);
			break;
		case ELevelLog::WARNING:
			EG_LOG_WARN("[{0} {1}]: {2}", categoryName.GetName(), function, text);
			break;
		}
	}
	template<class T, class ... Args>
	static void LogOutput(const LogCategory& categoryName, const ELevelLog levelLog, const char* function, const T& text, Args&& ... args)
	{
		switch (levelLog)
		{
		case ELevelLog::INFO:
			EG_LOG_INFO("[" + categoryName.GetName() + " " + function + "]: " + ToString(text), std::forward<Args>(args)...);
			break;
		case ELevelLog::ERROR:
			EG_LOG_ERROR("[" + categoryName.GetName() + " " + function + "]: " + ToString(text), std::forward<Args>(args)...);
			break;
		case ELevelLog::CRITICAL:
			EG_LOG_CRITICAL("[" + categoryName.GetName() + " " + function + "]: " + ToString(text), std::forward<Args>(args)...);
			break;
		case ELevelLog::WARNING:
			EG_LOG_WARN("[" + categoryName.GetName() + " " + function + "]: " + ToString(text), std::forward<Args>(args)...);
			break;
		}
	}
		
private:

	static SharedPtr<spdlog::logger> m_Log;
		
};


	
