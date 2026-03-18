#pragma once
#include <Render/includes/RenderDevice.h>
#include <Core/includes/Base.h>

namespace CoreEngine
{
	namespace Render
	{
		class VertexBufferObject;
		class ElementBufferObject;
		class VertexArrayObject;
		class Texture2D;
		class Shader;
	}
}


namespace CoreEngine::Render::OpenGL
{


	class OpenGLRenderDevice : public RenderDevice
	{
	public:

		using IDType = uint64;

	public:

		// Create VBO/IBO
		virtual RHI::BufferHandle CreateBuffer(const EBufferTargetType Target, const void* vertexArr, const uint32 sizeArr, const ETypeData& TypeArr, const ETypeStorageDraw typeDraw = ETypeStorageDraw::STATIC) override;
		virtual RHI::HandleVAO CreateVAO(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO, const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs) override;

		virtual RHI::TextureHandle CreateTexture2D(const uint32 Width, const uint32 Height, const ETypeChannel Channel, const void* Data) override;
		virtual RHI::TextureHandle CreateTexture2D(const char* Path) override;

		virtual RHI::ShaderHandle CreateShader(const StringView VertexSource, const StringView FragmentSource) override;

		virtual const VertexBufferObject* GetVertexBufferID(const RHI::BufferHandle& Handle) const override;
		virtual const ElementBufferObject* GetElementBufferID(const RHI::BufferHandle& Handle) const override;
		virtual const Texture2D* GetTextureID(const RHI::TextureHandle& Handle) const override;
		virtual const Shader* GetShaderID(const RHI::ShaderHandle& Handle) const override;
		virtual const VertexArrayObject* GetVAOID(const RHI::HandleVAO& Handle) const override;

		virtual bool DeleteVBO(RHI::BufferHandle& Handle) override;
		virtual bool DeleteeEBO(RHI::BufferHandle& Handle) override;
		virtual bool DeleteTexture2D(RHI::TextureHandle& Handle) override;
		virtual bool DeleteShader(RHI::ShaderHandle& Handle) override;
		virtual bool DeleteVAO(RHI::HandleVAO& Handle) override;

	private:

		IDType GenerateNewId();

	private:

		HashTableMap<IDType, UniquePtr<VertexBufferObject>> m_VBOBuffers;
		HashTableMap<IDType, UniquePtr<ElementBufferObject>> m_EBOBuffers;
		HashTableMap<IDType, UniquePtr<VertexArrayObject>> m_VAOBuffers;
		HashTableMap<IDType, UniquePtr<Texture2D>> m_Textures2D;
		HashTableMap<IDType, UniquePtr<Shader>> m_Shaders;

		uint64 ID = 1;
	};
}