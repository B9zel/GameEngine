#pragma once
#include <Render/includes/RenderHardwareInterface.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Render/includes/Texture.h>
#include <Render/includes/Enums/TypeData.h>

namespace CoreEngine::Render
{
	class ElementBufferObject;
	class VertexBufferObject;
	class Shader;
	class VertexArrayObject;

	enum class EBufferTargetType : uint8
	{
		VERTEX_BUFFER = 0,
		ELEMENT_BUFFER
	};

	class RenderDevice
	{
	public:

		// Create VBO/IBO
		virtual RHI::BufferHandle CreateBuffer(const EBufferTargetType Target, const void* vertexArr, const uint32 sizeArr, const ETypeData& TypeArr,
											   const ETypeStorageDraw typeDraw = ETypeStorageDraw::STATIC) = 0;

		virtual RHI::TextureHandle CreateTexture2D(const uint32 Width, const uint32 Height, const ETypeChannel Channel, const void* Data) = 0;
		virtual RHI::TextureHandle CreateTexture2D(const char* Path) = 0;

		virtual RHI::ShaderHandle CreateShader(const StringView VertexSource, const StringView FragmentSource) = 0;
		virtual RHI::HandleVAO CreateVAO(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO,
										 const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs) = 0;

		virtual const VertexBufferObject* GetVertexBufferID(const RHI::BufferHandle& Handle) const = 0;
		virtual const ElementBufferObject* GetElementBufferID(const RHI::BufferHandle& Handle) const = 0;
		virtual const Texture2D* GetTextureID(const RHI::TextureHandle& Handle) const = 0;
		virtual const Shader* GetShaderID(const RHI::ShaderHandle& Handle) const = 0;
		virtual const VertexArrayObject* GetVAOID(const RHI::HandleVAO& Handle) const = 0;

		virtual bool DeleteVBO(RHI::BufferHandle& Handle) = 0;
		virtual bool DeleteeEBO(RHI::BufferHandle& Handle) = 0;
		virtual bool DeleteTexture2D(RHI::TextureHandle& Handle) = 0;
		virtual bool DeleteShader(RHI::ShaderHandle& Handle) = 0;
		virtual bool DeleteVAO(RHI::HandleVAO& Handle) = 0;
	};
} // namespace CoreEngine::Render
