#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>





namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
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
				cachedParameters.clear();

				m_ID = otherShder.m_ID;
				m_IsCompile = otherShder.m_IsCompile;
				cachedParameters = std::move(otherShder.cachedParameters);
				m_NameTextures = std::move(otherShder.m_NameTextures);


				otherShder.m_ID = 0;
				otherShder.m_IsCompile = false;

				return *this;
			}

			bool OpenGLShader::CompileShader(const String& vertexShader, const String& fragmentShader)
			{
				bool isSuccess = true;

				uint32 l_vertexShader;
				uint32 l_fragmentShader;

				// Compile vertex shader
				l_vertexShader = glCreateShader(GL_VERTEX_SHADER);
				{
					const GLchar* const convert = vertexShader.c_str();
					glShaderSource(l_vertexShader, 1, &convert, NULL);
					glCompileShader(l_vertexShader);
				}

				int32 shaderSuccess;

				glGetShaderiv(l_vertexShader, GL_COMPILE_STATUS, &shaderSuccess);
				if (!shaderSuccess)
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


				glGetShaderiv(l_fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
				if (!shaderSuccess)
				{
					char LogInfo[1024];
					glGetShaderInfoLog(l_fragmentShader, 1024, NULL, LogInfo);

					EG_LOG(CORE, ELevelLog::ERROR, LogInfo);
					isSuccess = false;
				}

				// Link shader
				uint32 l_programShader;

				l_programShader = glCreateProgram();
				glAttachShader(l_programShader, l_vertexShader);
				glAttachShader(l_programShader, l_fragmentShader);
				glLinkProgram(l_programShader);

				glGetProgramiv(l_programShader, GL_LINK_STATUS, &shaderSuccess);
				if (!shaderSuccess)
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
					AnalysisTextureShader(vertexShader, fragmentShader);
					AnalysisMatrix4(vertexShader);
				}

				glDeleteShader(l_vertexShader);
				glDeleteShader(l_fragmentShader);

				return isSuccess;
			}

			const DArray<String>& OpenGLShader::GetNamesOfTexture() const
			{
				return m_NameTextures;
			}

			bool OpenGLShader::GetIsCompile()
			{
				return m_IsCompile;
			}

			bool OpenGLShader::GetHasAllMatrix()
			{
				return m_HasAllMatrix;
			}

			void OpenGLShader::Bind() const
			{
				glUseProgram(m_ID);
			}

			void OpenGLShader::UnBind() const
			{
				glUseProgram(0);
			}

			bool OpenGLShader::HasUniformLocation(const char* nameParam)
			{
				return GetUniformLocation(nameParam) >= 0;
			}

			int32 OpenGLShader::GetUniformLocation(const char* nameParam)
			{
				return glGetUniformLocation(m_ID, nameParam);
			}


			bool OpenGLShader::SetUniformMatrix4x4(const String& nameParam, const FMatrix4x4& matrix, bool isBindShader)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isBindShader)
					{
						Bind();
						glUniformMatrix4fv(location, 1, GL_FALSE, GetValuePtr(matrix));
						UnBind();
					}
					else
					{
						glUniformMatrix4fv(location, 1, GL_FALSE, GetValuePtr(matrix));
					}
				return true;
			}

			bool OpenGLShader::SetUniform1i(const String& nameParam, const int32 value, bool isEnableBind)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)
					if (isEnableBind)
					{
						Bind();
						glUniform1i(location, value);
						UnBind();
					}
					else
					{
						glUniform1i(location, value);
					}

				return true;
			}

			bool OpenGLShader::SetUniform1ui(const String& nameParam, const uint32 value, bool isEnableBind)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isEnableBind)
					{
						Bind();
						glUniform1ui(location, value);
						UnBind();
					}
					else
					{
						glUniform1ui(location, value);
					}
				return true;
			}

			bool OpenGLShader::SetUniformFloat(const String& nameParam, float value, bool isEnableBind)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isEnableBind)
					{
						Bind();
						glUniform1f(location, value);
						UnBind();
					}
					else
					{
						glUniform1f(location, value);
					}
				return true;
			}

			bool OpenGLShader::SetUniformVec4(const String& nameParam, const FVector4& vec, bool isEnableBind)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isEnableBind)
					{
						Bind();
						glUniform4f(location, vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW());
						UnBind();
					}
					else
					{
						glUniform4f(location, vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW());
					}
				return true;
			}

			bool OpenGLShader::SetUniformVec2(const String& nameParam, const FVector2& vec, bool isEnableBind)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isEnableBind)
					{
						Bind();
						glUniform2f(location, vec.x, vec.y);
						UnBind();
					}
					else
					{
						glUniform2f(location, vec.x, vec.y);
					}
				return true;
			}

			bool OpenGLShader::SetUniformVec3(const String& nameParam, const FVector& vec, bool isEnableBind)
			{
				int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isEnableBind)
					{
						Bind();
						glUniform3f(location, vec.GetX(), vec.GetY(), vec.GetZ());
						UnBind();
					}
					else
					{
						glUniform3f(location, vec.GetX(), vec.GetY(), vec.GetZ());
					}
				return true;
			}

			bool OpenGLShader::GetCachedLocationParam(const String& Key, int32& outLocation)
			{
				auto& it = cachedParameters.find(Key);
				if (it != cachedParameters.end())
				{
					outLocation = it->second;
					return true;
				}
				return false;
			}
			void OpenGLShader::AnalysisTextureShader(const String& vertex, const String& fragment)
			{
				auto Analysis = [this](const String& shader)
					{
						std::string_view searchLine = "uniform sampler";
						size_t Pos = shader.find(searchLine);
						while (Pos != String::npos)
						{
							if (IsInComment(shader, Pos))
							{
								Pos = shader.find(searchLine, Pos + 1);
								continue;
							}
							size_t endLine = shader.find_first_of(';', Pos); //shader.substr(Pos + searchLine.size(), a - Pos - searchLine.size());


							const String dimension = shader.substr(Pos + searchLine.size(), 2);
							if (dimension != "1D" && dimension != "2D" && dimension != "3D")
							{
								Pos = shader.find(searchLine, Pos + 1);
								continue;
							}

							size_t PosBeforeName = Pos + searchLine.size() + dimension.size();
							size_t sizeName = endLine - PosBeforeName;
							String Name = shader.substr(PosBeforeName, sizeName);
							Name.erase(std::remove(Name.begin(), Name.end(), ' '), Name.end());

							m_NameTextures.emplace_back(Name);
							Pos = shader.find(searchLine, Pos + 1);
						}
					};

				Analysis(fragment);
				Analysis(vertex);
			}

			void OpenGLShader::AnalysisMatrix4(const String& vertex)
			{
				auto Analysis = [this](const String& shader)
					{
						const std::string_view searchLine = "uniform mat4";
						HashTableMap<String, bool> Matrix = { {"Model", false},{"View", false}, {"Projection", false} };

						size_t Pos = shader.find(searchLine);
						while (Pos != String::npos)
						{
							if (IsInComment(shader, Pos))
							{
								Pos = shader.find(searchLine, Pos + 1);
								continue;
							}
							size_t endLine = shader.find_first_of(';', Pos); //shader.substr(Pos + searchLine.size(), a - Pos - searchLine.size());


							size_t PosBeforeName = Pos + searchLine.size();
							size_t sizeName = endLine - PosBeforeName;
							String Name = shader.substr(PosBeforeName, sizeName);
							Name.erase(std::remove(Name.begin(), Name.end(), ' '), Name.end());

							if (Matrix.count(Name))
							{
								Matrix[Name] = true;
							}

							Pos = shader.find(searchLine, Pos + 1);
						}

						bool hasMatrix = true;
						for (auto& el : Matrix)
						{
							if (!el.second)
							{
								hasMatrix = false;
							}
						}
						m_HasAllMatrix = hasMatrix;
					};

				Analysis(vertex);
			}


			bool OpenGLShader::IsInComment(const String& str, const size_t PosTarget)
			{
				// Check multi comment
				size_t OpenPos = str.find("/*");
				while (OpenPos != String::npos)
				{
					size_t ClosePos = str.find("*/", OpenPos);
					if (ClosePos != String::npos)
					{
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
		}
	}
}