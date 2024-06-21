#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>
#include <fstream>


namespace CoreEngine
{
	class FileManager
	{
		static String ReadFile(const char* path);
		static void WiteFile(const char* path, const String& text);
		static void AddFile(const char* path, const String& text);
	};
}
