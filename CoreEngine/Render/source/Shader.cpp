#include <Render/includes/Shader.h>
#include <Core/includes/FileManager.h>
#include <Render/includes/RendererAPI.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Render/includes/RenderDevice.h>

namespace CoreEngine
{
	namespace Render
	{
		// Init static variables
		const String Shader::VertexShaderPoint = "#type vertex";
		const String Shader::FragmentShaderPoint = "#type fragment";
		//

		/*Pair<String, String> Shader::LoadShader(const char* pathToShader)
		{
			const String& shader = FileManager::ReadFile(pathToShader);
			Pair<String, String> retur;

			size_t vertexPos = shader.find(VertexShaderPoint);
			if (vertexPos == String::npos)
			{
				EG_LOG(CORE, ELevelLog::ERROR, "The string \"{0}\" was't found", VertexShaderPoint);
				return Pair<String, String>();
			}

			size_t fragmentPos = shader.find(FragmentShaderPoint);
			if (fragmentPos == String::npos)
			{
				EG_LOG(CORE, ELevelLog::ERROR, "The string \"{0}\" was't found", FragmentShaderPoint);
				return Pair<String, String>();
			}

			retur.first = std::move(shader.substr(vertexPos + VertexShaderPoint.size(), fragmentPos - FragmentShaderPoint.size()));
			retur.second = std::move(shader.substr(fragmentPos + FragmentShaderPoint.size()));

			return retur;
		}*/

		UniquePtr<Shader> Shader::CreateShader()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeUniquePtr<OpenGL::OpenGLShader>();
			default:
				break;
			}
			EG_LOG(CORE, ELevelLog::CRITICAL, "No implament API to create");
		}
	} // namespace Render
} // namespace CoreEngine
