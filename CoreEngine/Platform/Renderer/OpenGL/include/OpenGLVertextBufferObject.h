#pragma once

#include <Render/includes/VertextBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>
#include <Core/includes/Base.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{
	namespace Render
	{

		class OpenGLVertexBufferObject : public VertexBufferObject
		{
		public:

			OpenGLVertexBufferObject();
			~OpenGLVertexBufferObject();
			
			OpenGLVertexBufferObject(const OpenGLVertexBufferObject&) = delete;
			OpenGLVertexBufferObject& operator=(const OpenGLVertexBufferObject&) = delete;

			OpenGLVertexBufferObject(OpenGLVertexBufferObject&& other) noexcept;
			OpenGLVertexBufferObject& operator=(OpenGLVertexBufferObject&& other) noexcept;


			virtual void CreaterBuffer(const void* vertexArr,unsigned int sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) override;

			virtual void DeleteBuffer() override;
			
			virtual void ChangeData(const int beginFillData,const unsigned int sizeOfData, const void* data,const bool isBind = true);

			virtual bool SetData(const void* vertexArr, unsigned int sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) override;

			virtual void Bind() const override;
			virtual void UnBind() const override;
			
			bool IsCreate();

		private:

			unsigned int m_VBO;
			ETypeData m_typeStorageData;
			bool m_IsCreate;
		};
	}

}