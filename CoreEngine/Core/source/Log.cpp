#include <Core/includes/Log.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>



SharedPtr<spdlog::logger> Log::m_Log;
	
void Log::Init()
{
	DArray<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	//logSinks.fron
	logSinks.front()->set_pattern("%^[%Y-%m-%d %H:%M:%S] [%l]%$ %v");
		
	m_Log = MakeSharedPtr<spdlog::logger>("Engine", begin(logSinks), end(logSinks));
	//m_Log->
	spdlog::register_logger(m_Log);
	m_Log->set_level(spdlog::level::trace);
	m_Log->flush_on(spdlog::level::trace);
}

