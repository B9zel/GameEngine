#include <Core/includes/FileManager.h>


namespace CoreEngine
{

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
    void FileManager::WiteFile(const char* path, const String& text)
    {
        std::ofstream file(path);
        file << text;

        file.close();
    }
    void FileManager::AddFile(const char* path, const String& text)
    {
        std::ofstream file(path, std::ios::app);
        file << text;

        file.close();
    }
}