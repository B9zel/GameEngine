#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
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
	} // namespace Render
} // namespace CoreEngine

namespace CoreEngine::Render::OpenGL
{

	class OpenGLRenderDevice : public RenderDevice
	{
	public:

		using IDType = uint64;

	public:

		// Create VBO/IBO
		virtual RHI::BufferHandle CreateBuffer(const EBufferTargetType Target, const void* DataArr, const uint32 sizeArr, const ETypeData& TypeArr,
											   const ETypeStorageDraw typeDraw = ETypeStorageDraw::STATIC) override;

		// @param Attribs: Location, SizeArgument, Step, TypeData, BeginStep
		virtual RHI::HandleVAO CreateVAO(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO,
										 const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs) override;

		virtual RHI::TextureHandle CreateTexture2D(const TextureDesc& Desc) override;
		virtual RHI::ShaderHandle CreateShader(const StringView VertexSource, const StringView FragmentSource) override;

		virtual const uint32 GetVertexBufferID(const RHI::BufferHandle& Handle) const override;
		virtual const uint32 GetElementBufferID(const RHI::BufferHandle& Handle) const override;
		virtual const uint32 GetTextureID(const RHI::TextureHandle& Handle) const override;
		virtual const uint32 GetShaderID(const RHI::ShaderHandle& Handle) const override;
		virtual const uint32 GetVAOID(const RHI::HandleVAO& Handle) const override;

		virtual bool SetUniformMatrix4x4(const RHI::ShaderHandle& Handle, const String& nameParam, const FMatrix4x4& matrix) override;
		virtual bool SetUniform1i(const RHI::ShaderHandle& Handle, const String& nameParam, const int32 value) override;
		virtual bool SetUniform1ui(const RHI::ShaderHandle& Handle, const String& nameParam, const uint32 value) override;
		virtual bool SetUniformFloat(const RHI::ShaderHandle& Handle, const String& nameParam, float value) override;
		virtual bool SetUniformVec4(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector4& vec) override;
		virtual bool SetUniformVec2(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector2& vec) override;
		virtual bool SetUniformVec3(const RHI::ShaderHandle& Handle, const String& nameParam, const FVector& vec) override;

		virtual void BindShader(const RHI::ShaderHandle& Handle) override;
		virtual void BindTexture(const RHI::TextureHandle& Handle, const uint32 Layout) override;
		virtual void BindVAO(const RHI::HandleVAO& Handle) override;
		virtual void BindVBO(const RHI::BufferHandle& Handle) override;
		virtual void BindEBO(const RHI::BufferHandle& Handle) override;

		virtual bool DeleteVBO(RHI::BufferHandle& Handle) override;
		virtual bool DeleteEBO(RHI::BufferHandle& Handle) override;
		virtual bool DeleteTexture2D(RHI::TextureHandle& Handle) override;
		virtual bool DeleteShader(RHI::ShaderHandle& Handle) override;
		virtual bool DeleteVAO(RHI::HandleVAO& Handle) override;

	private:

		IDType GenerateNewId();

	private:

		HashTableMap<IDType, uint32> m_VBOBuffers;
		HashTableMap<IDType, uint32> m_IBOBuffers;
		HashTableMap<IDType, uint32> m_VAOBuffers;
		HashTableMap<IDType, uint32> m_Textures2D;
		HashTableMap<IDType, uint32> m_Shaders;

		uint64 ID = 1;
	};
} // namespace CoreEngine::Render::OpenGL
