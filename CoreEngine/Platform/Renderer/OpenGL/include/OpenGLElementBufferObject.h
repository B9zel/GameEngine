#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/ElementBufferObject.h>
#include <glad/glad.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLElementBufferObject final : public ElementBufferObject
			{
			public:

				OpenGLElementBufferObject();
				~OpenGLElementBufferObject();

				OpenGLElementBufferObject(const OpenGLElementBufferObject&) = delete;
				OpenGLElementBufferObject(OpenGLElementBufferObject&& Other) noexcept;

			public:

				virtual void CreateBuffer(RenderDevice* Device, const void* dataArr, uint32 sizeArr, const ETypeData& typeData,
										  const ETypeStorageDraw& typeDraw, const VertexArrayObject& vertexArray, const VertexBufferObject& buffObj,
										  const bool IsAutoUnBind = true) override;
				virtual void CreateBuffer(RenderDevice* Device, const void* dataArr, uint32 sizeArr, const ETypeData& typeData,
										  const ETypeStorageDraw& typeDraw, const VertexArrayObject& vertexArray, const bool IsAutoUnBind = true) override;
				virtual void CreateBuffer(RenderDevice* Device, const void* dataArr, uint32 sizeArr, const ETypeData& typeData,
										  const ETypeStorageDraw& typeDraw, const bool IsAutoUnBind = true) override;
				virtual void DeleteBuffer() override;

				virtual bool SetData(const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind = true) override;

				virtual void Bind() const override;
				virtual void UnBind() const override;

				virtual RHI::BufferHandle GetHandle() const override;

				bool IsCreate() const
				{
					return m_isCreate;
				}

			private:

				uint32 m_EBO;
				ETypeData m_typeStorageData;
				bool m_isCreate;

				RHI::BufferHandle m_Handle;
			};

		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
