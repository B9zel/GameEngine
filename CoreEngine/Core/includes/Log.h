#pragma once
#include <spdlog/spdlog.h>
#include <Core/includes/Base.h>



namespace CoreEngine
{
	class Log;
	enum class ELevelLog;
	class LogCategory;

#ifdef _DEBUG

	#define EG_LOG_INFO(...) spdlog::info(__VA_ARGS__)
	#define EG_LOG_ERROR(...) spdlog::error(__VA_ARGS__)
	#define EG_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
	#define EG_LOG_WARN(...) spdlog::warn(__VA_ARGS__)
																		
	#define DECLARE_LOG_CATEGORY_EXTERN(Category) \
			static class LogCategory##Category : public CoreEngine::LogCategory \
			{														\
			public:													\
				LogCategory##Category() : LogCategory(#Category) {} \
			} Category;

	#define EG_LOG(Category,levelLog, ...) \
		{	\
			CoreEngine::Log::LogOutput(Category, levelLog, FUNCTION_STAT, __VA_ARGS__); \
		}

#else
	#define EG_LOG_INFO(...) 
	#define EG_LOG_ERROR(...)
	#define EG_LOG_CRITICAL(...) 
	#define EG_LOG_WARN(...) 

	#define DECLARE_LOG_CATEGORY_EXTERN(Category)
	#define EG_LOG(Category,levelLog, ...)
#endif // _DEBUG

	

	enum class ELevelLog : int
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

	
	class Log
	{
	public:

		template<class T>
		static void LogOutput(const LogCategory& categoryName, const ELevelLog levelLog, const char* function,const T& text)
		{
			switch (levelLog)
			{
			case ELevelLog::INFO:
				EG_LOG_INFO("[" + categoryName.GetName() + " " + function + "]: " + ToString(text));
				break;
			case ELevelLog::ERROR:
				EG_LOG_ERROR("[" + categoryName.GetName() + " " + function + "]: " + ToString(text));
				break;
			case ELevelLog::CRITICAL:
				EG_LOG_CRITICAL("[" + categoryName.GetName() + " " + function + "]: "  + ToString(text));
				break;
			case ELevelLog::WARNING:
				EG_LOG_WARN("[" + categoryName.GetName() + " " + function + "]: " + ToString(text));
				break;
			}
		}
		
	};


	
}

DECLARE_LOG_CATEGORY_EXTERN(CORE)
