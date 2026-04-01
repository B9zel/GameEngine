#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRenderDevice.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{

			/* #define SHADER_LOCATION_PARAM(KeyString, outLocation) \
				if (!GetCachedLocationParam(KeyString, outLocation)) \
				{ \
					if (!HasUniformLocation(KeyString.c_str())) \
					{ \
						EG_LOG(OPENGL_Shader, ELevelLog::ERROR, "Can't to find uniform {0}", KeyString.data()); \
						return false; \
					} \
					outLocation = GetUniformLocation(KeyString.c_str()); \
					cachedParameters.insert(std::pair(KeyString, outLocation)); \
				}*/

			OpenGLShader::OpenGLShader()
			{
				m_ID = 0;
				m_IsCompile = false;
			}

			OpenGLShader::~OpenGLShader()
			{
				glDeleteProgram(m_ID);
			}

			OpenGLShader::OpenGLShader(OpenGLShader&& otherShader) noexcept : OpenGLShader()
			{
				*this = std::move(otherShader);
			}

			OpenGLShader::OpenGLShader(RenderDevice* Device, const String& vertexShader, const String& fragmentShader) : OpenGLShader()
			{
				CompileShader(Device, vertexShader, fragmentShader);
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

			bool OpenGLShader::CompileShader(RenderDevice* Device, const String& vertexShader, const String& fragmentShader)
			{
				Handle = Device->CreateShader(vertexShader, fragmentShader);
				
				if (Handle.IsValid())
				{
					m_IsCompile = true;
					AnalysisTextureShader(vertexShader, fragmentShader);
					AnalysisMatrix4(vertexShader);
				}

				return Handle.IsValid();
			}

			bool OpenGLShader::CompileShader(RenderDevice* Device, const StringView vertexShader, const StringView fragmentShader)
			{
				static String vertex(vertexShader);
				static String fragment(fragmentShader);
				return CompileShader(Device, vertex, fragment);
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

			RHI::ShaderHandle OpenGLShader::GetHandle() const
			{
				return Handle;
			}

			void OpenGLShader::Bind(RenderDevice* Device) const
			{
				Device->BindShader(Handle);
			}

			void OpenGLShader::UnBind(RenderDevice* Device) const
			{
				// glUseProgram(0);
			}

			bool OpenGLShader::HasUniformLocation(const char* nameParam)
			{
				return GetUniformLocation(nameParam) >= 0;
			}

			int32 OpenGLShader::GetUniformLocation(const char* nameParam)
			{
				return glGetUniformLocation(m_ID, nameParam);
			}

			bool OpenGLShader::SetUniformMatrix4x4(RenderDevice* Device, const String& nameParam, const FMatrix4x4& matrix, bool isBindShader)
			{
				/*int32 location = 0;
				SHADER_LOCATION_PARAM(nameParam, location)

					if (isBindShader)
					{
						Bind();
						glUniformMatrix4fv(location, 1, GL_FALSE, Math::GetValuePtr(matrix));
						UnBind();
					}
					else
					{
						glUniformMatrix4fv(location, 1, GL_FALSE, Math::GetValuePtr(matrix));
					}*/

				return Device->SetUniformMatrix4x4(Handle, nameParam, matrix);
			}

			bool OpenGLShader::SetUniform1i(RenderDevice* Device, const String& nameParam, const int32 value, bool isEnableBind)
			{
				/*int32 location = 0;
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
					}*/

				return Device->SetUniform1i(Handle, nameParam, value);
			}

			bool OpenGLShader::SetUniform1ui(RenderDevice* Device, const String& nameParam, const uint32 value, bool isEnableBind)
			{
				/*int32 location = 0;
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
					}*/

				return Device->SetUniform1ui(Handle, nameParam, value);
			}

			bool OpenGLShader::SetUniformFloat(RenderDevice* Device, const String& nameParam, float value, bool isEnableBind)
			{
				/*int32 location = 0;
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
					}*/

				return Device->SetUniformFloat(Handle, nameParam, value);
			}

			bool OpenGLShader::SetUniformVec4(RenderDevice* Device, const String& nameParam, const FVector4& vec, bool isEnableBind)
			{
				/*int32 location = 0;
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
					}*/

				return Device->SetUniformVec4(Handle, nameParam, vec);
			}

			bool OpenGLShader::SetUniformVec2(RenderDevice* Device, const String& nameParam, const FVector2& vec, bool isEnableBind)
			{
				/*int32 location = 0;
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
					}*/

				return Device->SetUniformVec2(Handle, nameParam, vec);
			}

			bool OpenGLShader::SetUniformVec3(RenderDevice* Device, const String& nameParam, const FVector& vec, bool isEnableBind)
			{
				/*int32 location = 0;
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
					}*/

				return Device->SetUniformVec3(Handle, nameParam, vec);
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
						size_t endLine = shader.find_first_of(';', Pos); // shader.substr(Pos + searchLine.size(), a - Pos - searchLine.size());

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
					HashTableMap<String, bool> Matrix = {{"Model", false}, {"View", false}, {"Projection", false}};

					size_t Pos = shader.find(searchLine);
					while (Pos != String::npos)
					{
						if (IsInComment(shader, Pos))
						{
							Pos = shader.find(searchLine, Pos + 1);
							continue;
						}
						size_t endLine = shader.find_first_of(';', Pos); // shader.substr(Pos + searchLine.size(), a - Pos - searchLine.size());

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
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
