#pragma once

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

				virtual void CreaterBuffer(const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw, const VertexArrayObject& vertexArray) override;

				virtual void DeleteBuffer() override;

				virtual bool SetData(const int32 beginFillData, const uint32 sizeOfData, const void* data, const bool isBind = true) override;

				//virtual bool SetData(const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) override;

				virtual void Bind() const override;
				virtual void UnBind() const override;

				bool IsCreate();

			private:

				uint32 m_VBO;
				ETypeData m_typeStorageData;
				bool m_IsCreate;
			};
		}
	}

}