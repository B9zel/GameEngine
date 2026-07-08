#include <Render/includes/ShaderGLSLUtils.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>

namespace CoreEngine::Render::GLSL
{

	bool IsInComment(const String& str, const size_t PosTarget)
	{
		// Check multi comment
		size_t OpenPos = str.find("/*");
		while (OpenPos != String::npos)
		{
			size_t ClosePos = str.find("*/", OpenPos);
			if (ClosePos != String::npos)
			{
				if (OpenPos > PosTarget) break;

				if (PosTarget > OpenPos && PosTarget < ClosePos)
				{
					return true;
				}
				OpenPos = str.find("/*", ClosePos);
			}
			else
			{
				return true;
			}
		}

		// Check single comment
		size_t BeginComment = str.find("//");

		while (BeginComment != String::npos)
		{
			size_t NewLine = str.find("\n", BeginComment);
			if (NewLine != String::npos)
			{
				if (BeginComment <= PosTarget && PosTarget < NewLine)
				{
					return true;
				}
				BeginComment = str.find("//", NewLine);
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	void ParseShader(const String& shader, HashTableMap<String, UniformFileInfo>& outUniforms)
	{
		outUniforms.clear();
		const int32 Space = 1;

		size_t CurrentPos = 0;
		bool HasElement = true;
		while (HasElement)
		{
			HasElement = false;
			for (auto& SearchStr : OpenGL::OpenGLShader::SearchElements)
			{
				size_t Pos = shader.find(SearchStr, CurrentPos);
				if (Pos != SearchStr.npos)
				{
					HasElement = true;
					CurrentPos = Pos + Space;
					if (IsInComment(shader, Pos)) continue;

					size_t BeginTypePos = Pos + SearchStr.size() + Space;
					size_t PosBetweenTypeAndName = shader.find(" ", BeginTypePos);

					const String& NameVar =
						shader.substr(PosBetweenTypeAndName + Space, shader.find(";", PosBetweenTypeAndName) - PosBetweenTypeAndName - Space);

					const auto& FindedType = OpenGL::OpenGLShader::UniformTypStr.find(shader.substr(BeginTypePos, PosBetweenTypeAndName - BeginTypePos));
					if (FindedType != OpenGL::OpenGLShader::UniformTypStr.end())
					{
						UniformFileInfo Info;
						Info.Type = FindedType->second;
						outUniforms.emplace(NameVar, Info);
					}
				}
			}
		}
	}

} // namespace CoreEngine::Render::GLSL
