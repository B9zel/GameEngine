#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/VertextBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>
#include <Render/includes/VertexArrayObject.h>
#include <Core/includes/Base.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLVertexBufferObject final : public VertexBufferObject
			{
			public:

				OpenGLVertexBufferObject();
				~OpenGLVertexBufferObject();

				OpenGLVertexBufferObject(const OpenGLVertexBufferObject&) = delete;
				OpenGLVertexBufferObject& operator=(const OpenGLVertexBufferObject&) = delete;

				OpenGLVertexBufferObject(OpenGLVertexBufferObject&& other) noexcept;
				OpenGLVertexBufferObject& operator=(OpenGLVertexBufferObject&& other) noexcept;

			public:

				virtual void CreateBuffer(RenderDevice* Device, const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr,
										  const ETypeStorageDraw& typeDraw, const VertexArrayObject& vertexArray, const bool IsAutoUnBind = true) override;
				virtual void CreateBuffer(RenderDevice* Device, const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr,
										  const ETypeStorageDraw& typeDraw, const bool IsAutoUnBind = true) override;

				virtual void DeleteBuffer() override;

				virtual bool SetData(RenderDevice* Device, const int32 beginFillData, const uint32 sizeOfData, const void* data,
									 const bool isBind = true) override;

				// virtual bool SetData(const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) override;

				virtual void Bind(RenderDevice* Device) const override;
				virtual void UnBind() const override;

				virtual RHI::BufferHandle GetHandle() const override;

				bool IsCreate();

			private:

				uint32 m_VBO;
				ETypeData m_typeStorageData;
				bool m_IsCreate;

				RHI::BufferHandle m_Handle;
			};
		} // namespace OpenGL
	} // namespace Render

} // namespace CoreEngine
