#include <Platform/Renderer/OpenGL/include/OpenGLRenderDevice.h>
#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Render/includes/Shader.h>
#include <Render/includes/VertextBufferObject.h>
#include <Core/includes/Base.h>
#include <Math/includes/Matrix.h>
#include <Math/includes/Vector.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>
#include "glad/glad.h"

DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog);
DECLARE_LOG_CATEGORY_EXTERN(GLRenderDeviceLog);

namespace CoreEngine::Render::OpenGL
{
	RHI::BufferHandle OpenGLRenderDevice::CreateBuffer(const EBufferTargetType Target, const void* DataArr, const uint32 sizeArr, const ETypeData& TypeArr,
													   const ETypeStorageDraw typeDraw)
	{
		RHI::BufferHandle Handle;

		switch (Target)
		{
		case EBufferTargetType::VERTEX_BUFFER:
		{
			uint32 NewVBO;
			glGenBuffers(1, &NewVBO);
			glBindBuffer(GL_ARRAY_BUFFER, NewVBO);
			glBufferData(GL_ARRAY_BUFFER, GetSizeOfFromEnum(TypeArr) * sizeArr, DataArr, GetDrawTypeAPIFromEnum(typeDraw));

			IDType GenID = GenerateNewId();
			Handle.IdHandle = GenID;

			m_VBOBuffers.insert(std::make_pair(GenID, NewVBO));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			break;
		}
		case EBufferTargetType::ELEMENT_BUFFER:
		{
			uint32 NewEBO;
			glGenBuffers(1, &NewEBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSizeOfFromEnum(TypeArr) * sizeArr, DataArr, GetDrawTypeAPIFromEnum(typeDraw));

			IDType GenID = GenerateNewId();
			Handle.IdHandle = GenID;

			m_IBOBuffers.insert(std::make_pair(GenID, NewEBO));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			break;
		}
		}

		return Handle;
	}

	RHI::TextureHandle OpenGLRenderDevice::CreateTexture2D(const TextureDesc& Desc)
	{
		uint32 IdTex;
		glGenTextures(1, &IdTex);
		glBindTexture(GL_TEXTURE_2D, IdTex);

		uint32 Format = GL_RGB;
		if (Desc.Channels == 4)
		{
			Format = GL_RGBA;
		}
		uint32 MaxFilter = OpenGL::OpenGLTexture2D::GetFilterFromEnum(Desc.MaxFilter);
		uint32 MinFilter = OpenGL::OpenGLTexture2D::GetFilterFromEnum(Desc.MinFilter);
		if (Desc.GenerateMips)
		{
			MinFilter = GL_LINEAR_MIPMAP_LINEAR;
		}
		uint32 WrapS = OpenGL::OpenGLTexture2D::GetWrapFromEnum(Desc.WrapS);
		uint32 WrapT = OpenGL::OpenGLTexture2D::GetWrapFromEnum(Desc.WrapT);

		glTexImage2D(GL_TEXTURE_2D, 0, Format, Desc.Width, Desc.Height, 0, Format, GL_UNSIGNED_BYTE, Desc.Data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MaxFilter);

		if (Desc.GenerateMips)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		RHI::TextureHandle Handle;
		IDType GenID = GenerateNewId();
		Handle.IdHandle = GenID;
		m_Textures2D.insert(std::make_pair(GenID, IdTex));

		return Handle;
	}

	RHI::ShaderHandle OpenGLRenderDevice::CreateShader(const StringView VertexSource, const StringView FragmentSource)
	{
		RHI::ShaderHandle Handle;
		/*	auto& Shader = Shader::CreateShader();
			Shader->CompileShader(VertexSource, FragmentSource);*/

		uint32 l_vertexShader;
		uint32 l_fragmentShader;

		// Compile vertex shader
		l_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		{
			const GLchar* const convert = VertexSource.data();
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
		}

		// Compile fragment shader

		l_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* const convert = FragmentSource.data();
		glShaderSource(l_fragmentShader, 1, &convert, NULL);
		glCompileShader(l_fragmentShader);

		glGetShaderiv(l_fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
		if (!shaderSuccess)
		{
			char LogInfo[1024];
			glGetShaderInfoLog(l_fragmentShader, 1024, NULL, LogInfo);

			EG_LOG(CORE, ELevelLog::ERROR, LogInfo);
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
		}
		else
		{
			IDType GenID = GenerateNewId();
			Handle.IdHandle = GenID;
			m_Shaders.insert(std::make_pair(GenID, l_programShader));
		}

		glDeleteShader(l_vertexShader);
		glDeleteShader(l_fragmentShader);

		return Handle;
	}

	RHI::HandleVAO OpenGLRenderDevice::CreateVAO(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO,
												 const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs)
	{
		RHI::HandleVAO Handle;
		if (!VBO.IsValid() || !IBO.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't create VAO, VBO or IBO is invalid");
			return RHI::HandleVAO();
		}

		uint32 NewVAO;
		glGenVertexArrays(1, &NewVAO);
		glBindVertexArray(NewVAO);
		glBindBuffer(GL_ARRAY_BUFFER, GetVertexBufferID(VBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetElementBufferID(IBO));

		for (auto& Attrib : Attribs)
		{
			uint32 Location, SizeArgument, Step, BeginStep;
			ETypeData TypeData;
			std::tie(Location, SizeArgument, Step, TypeData, BeginStep) = Attrib;

			if (TypeData == ETypeData::INT || TypeData == ETypeData::UNSIGNED_INT)
			{
				glVertexAttribIPointer(Location, SizeArgument, GetAPITypeFromEnum(TypeData), Step, (GLvoid*)(BeginStep));
			}
			else
			{
				glVertexAttribPointer(Location, SizeArgument, GetAPITypeFromEnum(TypeData), GL_FALSE, Step, // * GetSizeOfFromEnum(TypeData)
									  (GLvoid*)(BeginStep));
			}
			glEnableVertexAttribArray(Location);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		uint64 GenID = GenerateNewId();
		Handle.IdHandle = GenID;
		m_VAOBuffers.insert(std::make_pair(GenID, NewVAO));

		return Handle;
	}

	const uint32 OpenGLRenderDevice::GetVertexBufferID(const RHI::BufferHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return 0;
		}

		if (auto& It = m_VBOBuffers.find(Handle.GetId()); It != m_VBOBuffers.end())
		{
			return It->second;
		}

		return 0;
	}

	const uint32 OpenGLRenderDevice::GetElementBufferID(const RHI::BufferHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return 0;
		}

		if (auto& It = m_IBOBuffers.find(Handle.GetId()); It != m_IBOBuffers.end())
		{
			return It->second;
		}

		return 0;
	}

	const uint32 OpenGLRenderDevice::GetTextureID(const RHI::TextureHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return 0;
		}

		if (auto& It = m_Textures2D.find(Handle.GetId()); It != m_Textures2D.end())
		{
			return It->second;
		}

		return 0;
	}

	const uint32 OpenGLRenderDevice::GetShaderID(const RHI::ShaderHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return 0;
		}

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			return It->second;
		}

		return 0;
	}

	const uint32 OpenGLRenderDevice::GetVAOID(const RHI::HandleVAO& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return 0;
		}

		if (auto& It = m_VAOBuffers.find(Handle.GetId()); It != m_VAOBuffers.end())
		{
			return It->second;
		}

		return 0;
	}

	bool OpenGLRenderDevice::SetUniformMatrix4x4(const RHI::ShaderHandle& Handle, const String& nameParam, const FMatrix4x4& matrix)
	{
		if (!Handle.IsValid()) return false;

		auto It = m_Shaders.find(Handle.GetId());
		if (It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniformMatrix4fv(Location, 1, GL_FALSE, Math::GetValuePtr(matrix));

			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::SetUniform1i(const RHI::ShaderHandle& Handle, const String& nameParam, const int32 value)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniform1i(Location, value);

			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::SetUniform1ui(const RHI::ShaderHandle& Handle, const String& nameParam, const uint32 value)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniform1ui(Location, value);

			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::SetUniformFloat(const RHI::ShaderHandle& Handle, const String& nameParam, float value)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniform1f(Location, value);

			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::SetUniformVec4(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector4& vec)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniform4f(Location, vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW());

			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::SetUniformVec2(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector2& vec)
	{
		if (!Handle.IsValid()) return false;

		auto It = m_Shaders.find(Handle.GetId());
		if (It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniform2f(Location, vec.x, vec.y);

			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::SetUniformVec3(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector& vec)
	{
		if (!Handle.IsValid()) return false;

		auto It = m_Shaders.find(Handle.GetId());
		if (It != m_Shaders.end())
		{
			int32 Location = glGetUniformLocation(It->second, nameParam.c_str());
			glUniform3f(Location, vec.GetX(), vec.GetY(), vec.GetZ());

			return true;
		}

		return false;
	}

	void OpenGLRenderDevice::BindShader(const RHI::ShaderHandle& Handle)
	{
		if (!Handle.IsValid()) return;

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			glUseProgram(It->second);
			return;
		}
		ASSERT("Handle isn't exist");
	}

	void OpenGLRenderDevice::BindTexture(const RHI::TextureHandle& Handle, const uint32 Layout)
	{
		if (auto& It = m_Textures2D.find(Handle.GetId()); It != m_Textures2D.end())
		{
			glActiveTexture(GL_TEXTURE0 + Layout);
			glBindTexture(GL_TEXTURE_2D, It->second);
		}
	}

	void OpenGLRenderDevice::BindVAO(const RHI::HandleVAO& Handle)
	{
		if (!Handle.IsValid()) return;

		if (auto& It = m_VAOBuffers.find(Handle.GetId()); It != m_VAOBuffers.end())
		{
			glBindVertexArray(It->second);
			return;
		}
		ASSERT("Handle isn't exist");
	}

	void OpenGLRenderDevice::BindVBO(const RHI::BufferHandle& Handle)
	{
		if (!Handle.IsValid()) return;

		if (auto& It = m_VBOBuffers.find(Handle.GetId()); It != m_VBOBuffers.end())
		{
			glBindBuffer(GL_ARRAY_BUFFER, It->second);
			return;
		}
		ASSERT("Handle isn't exist");
	}

	void OpenGLRenderDevice::BindEBO(const RHI::BufferHandle& Handle)
	{
		if (!Handle.IsValid()) return;

		if (auto& It = m_IBOBuffers.find(Handle.GetId()); It != m_IBOBuffers.end())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, It->second);
			return;
		}
		ASSERT("Handle isn't exist");
	}

	bool OpenGLRenderDevice::DeleteVBO(RHI::BufferHandle& Handle)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_VBOBuffers.find(Handle.GetId()); It != m_VBOBuffers.end())
		{
			m_VBOBuffers.erase(It);
			Handle.Invalide();
			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::DeleteEBO(RHI::BufferHandle& Handle)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_IBOBuffers.find(Handle.GetId()); It != m_IBOBuffers.end())
		{
			m_IBOBuffers.erase(It);
			Handle.Invalide();
			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::DeleteTexture2D(RHI::TextureHandle& Handle)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_Textures2D.find(Handle.GetId()); It != m_Textures2D.end())
		{
			glDeleteTextures(1, &It->second);

			m_Textures2D.erase(It);
			Handle.Invalide();
			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::DeleteShader(RHI::ShaderHandle& Handle)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			glDeleteProgram(It->second);

			m_Shaders.erase(It);
			Handle.Invalide();
			return true;
		}

		return false;
	}

	bool OpenGLRenderDevice::DeleteVAO(RHI::HandleVAO& Handle)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_VAOBuffers.find(Handle.GetId()); It != m_VAOBuffers.end())
		{
			m_VAOBuffers.erase(It);
			Handle.Invalide();
			return true;
		}

		return false;
	}

	OpenGLRenderDevice::IDType OpenGLRenderDevice::GenerateNewId()
	{
		return ID++;
	}
} // namespace CoreEngine::Render::OpenGL
