#include <Core/includes/FileManager.h>
#include <Core/includes/Memory/SaveManager.h>
#include <filesystem>

namespace CoreEngine
{
	DECLARE_LOG_CATEGORY_EXTERN(FileManagerLog)

	String FileManager::ReadFile(const char* path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			EG_LOG(CORE, ELevelLog::WARNING, "Can't open file: {0}", path);
			return String();
		}
		std::stringstream buffer;
		buffer << file.rdbuf();

		file.close();

		return buffer.str();
	}
	void FileManager::WriteFile(const char* path, const String& text)
	{
		std::ofstream file(path);
		file << text;

		file.close();
	}
	void FileManager::AddInFile(const char* path, const String& text)
	{
		std::ofstream file(path, std::ios::app);
		file << text;

		file.close();
	}
	String FileManager::RenameFile(const String& Path, const String& NewName)
	{
		size_t SlashPos = Path.find_last_of("/\\");
		if (SlashPos == Path.npos)
		{
			return "";
		}
		String NewPath = Path.substr(0, SlashPos);
		NewPath += "/" + NewName + SaveManager::FileExtension;
		try
		{
			std::filesystem::rename(Path, NewPath);
		}
		catch (const std::filesystem::filesystem_error& Error)
		{
			EG_LOG(FileManagerLog, ELevelLog::WARNING, "Can't rename file {}", Path);
			return "";
		}
		return std::filesystem::path(NewPath).string();
	}
} // namespace CoreEngine
