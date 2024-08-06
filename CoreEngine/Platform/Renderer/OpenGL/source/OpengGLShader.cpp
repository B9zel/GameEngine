#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>





namespace CoreEngine
{
	namespace Render
	{
		#define SHADER_LOCATION_PARAM(KeyString, outLocation)															\
		if (!GetCachedLocationParam(KeyString, outLocation))															\
		{																												\
			if (!HasUniformLocation(KeyString.c_str()))																	\
			{																											\
				EG_LOG(OPENGL_Shader, ELevelLog::ERROR, "Can't to find uniform {0}", KeyString.data());					\
				return false;																							\
			}																											\
			outLocation = GetUniformLocation(KeyString.c_str());														\
			cachedParameters.insert(std::pair(KeyString, outLocation));													\
		}\



		OpenGLShader::OpenGLShader()
		{
			m_ID = 0;
			m_IsCompile = false;
		}

		OpenGLShader::OpenGLShader(OpenGLShader&& otherShader) noexcept : OpenGLShader()
		{
			*this = std::move(otherShader);
		}

		OpenGLShader::OpenGLShader(const String& vertexShader, const String& fragmentShader) : OpenGLShader()
		{
			CompileShader(vertexShader, fragmentShader);
		}

		OpenGLShader& OpenGLShader::operator=(OpenGLShader&& otherShder) noexcept
		{
			if (GetIsCompile())
			{
				glDeleteProgram(m_ID);
			}

			m_ID = otherShder.m_ID;
			m_IsCompile = otherShder.m_IsCompile;

			otherShder.m_ID = 0;
			otherShder.m_IsCompile = false;

			return *this;
		}

		bool OpenGLShader::CompileShader(const String& vertexShader, const String& fragmentShader)
		{
			bool isSuccess = true;

			unsigned int l_vertexShader;
			unsigned int l_fragmentShader;

			// Compile vertex shader
			l_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			{
				const GLchar* const convert = vertexShader.c_str();
				glShaderSource(l_vertexShader, 1, &convert, NULL);
				glCompileShader(l_vertexShader);
			}

			int shaderSucceess;

			glGetShaderiv(l_vertexShader, GL_COMPILE_STATUS, &shaderSucceess);
			if (!shaderSucceess)
			{
				char LogInfo[1024];
				glGetShaderInfoLog(l_vertexShader, 1024, NULL, LogInfo);

				EG_LOG(CORE, ELevelLog::ERROR, LogInfo);
				isSuccess = false;
			}

			// Compile fragment shader

			l_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const GLchar* const convert = fragmentShader.c_str();
			glShaderSource(l_fragmentShader, 1, &convert, NULL);
			glCompileShader(l_fragmentShader);


			glGetShaderiv(l_fragmentShader, GL_COMPILE_STATUS, &shaderSucceess);
			if (!shaderSucceess)
			{
				char LogInfo[1024];
				glGetShaderInfoLog(l_fragmentShader, 1024, NULL, LogInfo);

				EG_LOG(CORE, ELevelLog::ERROR, LogInfo);
				isSuccess = false;
			}

			// Link shader
			unsigned int l_programShader;

			l_programShader = glCreateProgram();
			glAttachShader(l_programShader, l_vertexShader);
			glAttachShader(l_programShader, l_fragmentShader);
			glLinkProgram(l_programShader);

			glGetProgramiv(l_programShader, GL_LINK_STATUS, &shaderSucceess);
			if (!shaderSucceess)
			{
				char LogInfo[1024];
				glGetProgramInfoLog(l_programShader, 1024, NULL, LogInfo);

				EG_LOG(CORE, ELevelLog::ERROR, LogInfo);
				glDeleteProgram(l_programShader);
				isSuccess = false;
			}
			else
			{
				m_ID = l_programShader;
				m_IsCompile = true;
			}

			glDeleteShader(l_vertexShader);
			glDeleteShader(l_fragmentShader);

			return isSuccess;
		}

		bool OpenGLShader::GetIsCompile()
		{
			return m_IsCompile;
		}

		void OpenGLShader::Bind()
		{
			glUseProgram(m_ID);
		}

		void OpenGLShader::UnBind()
		{
			glUseProgram(0);
		}
		bool OpenGLShader::HasUniformLocation(const char* nameParam)
		{
			return GetUniformLocation(nameParam) >= 0;
		}
		int OpenGLShader::GetUniformLocation(const char* nameParam)
		{
			return glGetUniformLocation(m_ID, nameParam);
		}


		bool OpenGLShader::SetUniformMatrix4x4(const String& nameParam,const FMatrix4x4& matrix)
		{
			int location = 0;
			SHADER_LOCATION_PARAM(nameParam, location)


			Bind();
			glUniformMatrix4fv(location, 1, GL_FALSE, GetValuePtr(matrix));
			UnBind();

			return true;
		}

		bool OpenGLShader::SetUniformFloat(const String& nameParam, float value)
		{
			int location = 0;
			SHADER_LOCATION_PARAM(nameParam, location)

			Bind();
			glUniform1f(location, value);
			UnBind();
		}

		bool OpenGLShader::SetUniformVec4(const String& nameParam,const FVector4& vec)
		{
			int location = 0;
			SHADER_LOCATION_PARAM(nameParam, location)

			Bind();
			glUniform4f(location, vec.x, vec.y, vec.z, vec.a);
			UnBind();
		}
		
		bool OpenGLShader::SetUniformVec2(const String& nameParam, const FVector2& vec)
		{
			int location = 0;
			SHADER_LOCATION_PARAM(nameParam, location)

			Bind();
			glUniform2f(location, vec.x, vec.y);
			UnBind();
		}

		bool OpenGLShader::GetCachedLocationParam(const String& Key, int& outLocation)
		{
			auto& it = cachedParameters.find(Key);
			if (it != cachedParameters.end())
			{
				outLocation = it->second;
				return true;
			}
			return false;
		}
	}
}