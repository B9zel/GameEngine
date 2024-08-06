#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>
#include <fstream>


namespace CoreEngine
{
	class FileManager
	{
	public:

		static String ReadFile(const char* path);
		static void WriteFile(const char* path, const String& text);
		static void AddInFile(const char* path, const String& text);
	};
}
