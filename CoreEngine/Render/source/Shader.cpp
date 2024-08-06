#include <Render/includes/Shader.h>
#include <Core/includes/FileManager.h>


Pair<String, String> CoreEngine::Render::Shader::LoadShader(const char* pathToShader)
{
    const String& shader = FileManager::ReadFile(pathToShader);
    const String vertexShaderPoint = "#type vertex";
    const String fragmentShaderPoint = "#type fragment";
    Pair<String, String> retur;

    size_t vertexPos = shader.find(vertexShaderPoint);
    if (vertexPos == String::npos)
    {
        EG_LOG(CORE, ELevelLog::ERROR, "The string \"{0}\" was't found", vertexShaderPoint);
        return Pair<String, String>();
    }

    size_t fragmentPos = shader.find(fragmentShaderPoint);
    if (fragmentPos == String::npos)
    {
        EG_LOG(CORE, ELevelLog::ERROR, "The string \"{0}\" was't found", fragmentShaderPoint);
        return Pair<String, String>();
    }

    
    retur.first = std::move(shader.substr(vertexPos + vertexShaderPoint.size(), fragmentPos - fragmentShaderPoint.size()));
    retur.second = std::move(shader.substr(fragmentPos + fragmentShaderPoint.size()));

    return retur;
}
