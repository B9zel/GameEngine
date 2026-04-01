#pragma once
#include <Render/includes/RenderHardwareInterface.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Render/includes/Texture.h>
#include <Render/includes/Enums/TypeData.h>
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>

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
		virtual RHI::BufferHandle CreateBuffer(const EBufferTargetType Target, const void* DataArr, const uint32 sizeArr, const ETypeData& TypeArr,
											   const ETypeStorageDraw typeDraw = ETypeStorageDraw::STATIC) = 0;

		virtual RHI::TextureHandle CreateTexture2D(const TextureDesc& Desc) = 0;

		virtual RHI::ShaderHandle CreateShader(const StringView VertexSource, const StringView FragmentSource) = 0;
		virtual RHI::HandleVAO CreateVAO(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO,
										 const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs) = 0;
		virtual void BindTexture(const RHI::TextureHandle& Handle, const uint32 Layout) = 0;
		virtual void BindVAO(const RHI::HandleVAO& Handle) = 0;
		virtual void BindVBO(const RHI::BufferHandle& Handle) = 0;
		virtual void BindEBO(const RHI::BufferHandle& Handle) = 0;

		virtual const uint32 GetVertexBufferID(const RHI::BufferHandle& Handle) const = 0;
		virtual const uint32 GetElementBufferID(const RHI::BufferHandle& Handle) const = 0;
		virtual const uint32 GetTextureID(const RHI::TextureHandle& Handle) const = 0;
		virtual const uint32 GetShaderID(const RHI::ShaderHandle& Handle) const = 0;
		virtual const uint32 GetVAOID(const RHI::HandleVAO& Handle) const = 0;

		// Shader
		virtual bool SetUniformMatrix4x4(const RHI::ShaderHandle& Handle, const String& nameParam, const FMatrix4x4& matrix) = 0;
		virtual bool SetUniform1i(const RHI::ShaderHandle& Handle, const String& nameParam, const int32 value) = 0;
		virtual bool SetUniform1ui(const RHI::ShaderHandle& Handle, const String& nameParam, const uint32 value) = 0;
		virtual bool SetUniformFloat(const RHI::ShaderHandle& Handle, const String& nameParam, float value) = 0;
		virtual bool SetUniformVec4(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector4& vec) = 0;
		virtual bool SetUniformVec2(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector2& vec) = 0;
		virtual bool SetUniformVec3(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector& vec) = 0;

		virtual void BindShader(const RHI::ShaderHandle& Handle) = 0;
		//

		virtual bool DeleteVBO(RHI::BufferHandle& Handle) = 0;
		virtual bool DeleteEBO(RHI::BufferHandle& Handle) = 0;
		virtual bool DeleteTexture2D(RHI::TextureHandle& Handle) = 0;
		virtual bool DeleteShader(RHI::ShaderHandle& Handle) = 0;
		virtual bool DeleteVAO(RHI::HandleVAO& Handle) = 0;
	};
} // namespace CoreEngine::Render
