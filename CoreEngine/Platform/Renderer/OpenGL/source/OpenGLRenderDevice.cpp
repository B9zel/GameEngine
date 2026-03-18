#include <Platform/Renderer/OpenGL/include/OpenGLRenderDevice.h>
#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Render/includes/Shader.h>
#include <Render/includes/VertextBufferObject.h>

DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog);

namespace CoreEngine::Render::OpenGL
{
	RHI::BufferHandle OpenGLRenderDevice::CreateBuffer(const EBufferTargetType Target, const void* vertexArr, const uint32 sizeArr, const ETypeData& TypeArr,
													   const ETypeStorageDraw typeDraw)
	{
		RHI::BufferHandle Handle;

		switch (Target)
		{
		case EBufferTargetType::VERTEX_BUFFER:
		{
			auto& VertexBuffer = VertexBufferObject::CreateVertexBufferObject();
			VertexBuffer->CreateBuffer(vertexArr, sizeArr, TypeArr, typeDraw, false);

			IDType GenID = GenerateNewId();
			Handle.IdHandle = GenID;

			m_VBOBuffers.insert(std::make_pair(GenID, std::move(VertexBuffer)));
			break;
		}
		case EBufferTargetType::ELEMENT_BUFFER:
		{
			auto& ElementBuffer = ElementBufferObject::CreateElementBuffer();
			ElementBuffer->CreateBuffer(vertexArr, sizeArr, TypeArr, typeDraw, false);

			IDType GenID = GenerateNewId();
			Handle.IdHandle = GenID;

			m_EBOBuffers.insert(std::make_pair(GenID, std::move(ElementBuffer)));
			break;
		}
		}

		return Handle;
	}

	RHI::TextureHandle OpenGLRenderDevice::CreateTexture2D(const uint32 Width, const uint32 Height, const ETypeChannel Channel, const void* Data)
	{
		RHI::TextureHandle Handle;
		auto& Texture = Texture2D::Create();
		Texture->RecreateTexture(Width, Height, Channel, Data);

		IDType GenID = GenerateNewId();
		Handle.IdHandle = GenID;
		m_Textures2D.insert(std::make_pair(GenID, std::move(Texture)));

		return Handle;
	}

	RHI::TextureHandle OpenGLRenderDevice::CreateTexture2D(const char* Path)
	{
		RHI::TextureHandle Handle;
		auto& Texture = Texture2D::Create(Path);

		IDType GenID = GenerateNewId();
		Handle.IdHandle = GenID;
		m_Textures2D.insert(std::make_pair(GenID, std::move(Texture)));

		return Handle;
	}

	RHI::ShaderHandle OpenGLRenderDevice::CreateShader(const StringView VertexSource, const StringView FragmentSource)
	{
		RHI::ShaderHandle Handle;
		auto& Shader = Shader::CreateShader();
		Shader->CompileShader(VertexSource, FragmentSource);

		IDType GenID = GenerateNewId();
		Handle.IdHandle = GenID;
		m_Shaders.insert(std::make_pair(GenID, std::move(Shader)));

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

		auto& VAO = VertexArrayObject::CreateVertexObject();
		GetVertexBufferID(VBO)->Bind();

		for (auto& Attrib : Attribs)
		{
			uint32 Location, SizeArgument, Step, BeginStep;
			ETypeData TypeData;
			std::tie(Location, SizeArgument, Step, TypeData, BeginStep) = Attrib;

			VAO->SetupIntorprit(Location, SizeArgument, Step, TypeData, BeginStep);
		}
		GetVertexBufferID(IBO)->Bind();
		VAO->UnBind();

		uint64 GenID = GenerateNewId();
		Handle.IdHandle = GenID;
		m_VAOBuffers.insert(std::make_pair(GenID, std::move(VAO)));

		return Handle;
	}

	const VertexBufferObject* OpenGLRenderDevice::GetVertexBufferID(const RHI::BufferHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return nullptr;
		}

		if (auto& It = m_VBOBuffers.find(Handle.GetId()); It != m_VBOBuffers.end())
		{
			return It->second.get();
		}

		return nullptr;
	}

	const ElementBufferObject* OpenGLRenderDevice::GetElementBufferID(const RHI::BufferHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return nullptr;
		}

		if (auto& It = m_EBOBuffers.find(Handle.GetId()); It != m_EBOBuffers.end())
		{
			return It->second.get();
		}

		return nullptr;
	}

	const Texture2D* OpenGLRenderDevice::GetTextureID(const RHI::TextureHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return nullptr;
		}

		if (auto& It = m_Textures2D.find(Handle.GetId()); It != m_Textures2D.end())
		{
			return It->second.get();
		}

		return nullptr;
	}

	const Shader* OpenGLRenderDevice::GetShaderID(const RHI::ShaderHandle& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return nullptr;
		}

		if (auto& It = m_Shaders.find(Handle.GetId()); It != m_Shaders.end())
		{
			return It->second.get();
		}

		return nullptr;
	}

	const VertexArrayObject* OpenGLRenderDevice::GetVAOID(const RHI::HandleVAO& Handle) const
	{
		if (!Handle.IsValid())
		{
			EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't get Buffer ID, Handle is invalid");
			return nullptr;
		}

		if (auto& It = m_VAOBuffers.find(Handle.GetId()); It != m_VAOBuffers.end())
		{
			return It->second.get();
		}

		return nullptr;
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

	bool OpenGLRenderDevice::DeleteeEBO(RHI::BufferHandle& Handle)
	{
		if (!Handle.IsValid()) return false;

		if (auto& It = m_EBOBuffers.find(Handle.GetId()); It != m_EBOBuffers.end())
		{
			m_EBOBuffers.erase(It);
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
