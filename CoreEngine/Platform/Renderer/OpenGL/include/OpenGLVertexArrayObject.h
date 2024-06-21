#pragma once

#include <Render/includes/VertexArrayObject.h>
#include <Core/includes/Base.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace CoreEngine
{
	namespace Render
	{

		class OpenGLVertexArrayObject : public VertexArrayObject
		{
		public:

			OpenGLVertexArrayObject();
			~OpenGLVertexArrayObject();

			OpenGLVertexArrayObject(const OpenGLVertexArrayObject&) = delete;
			OpenGLVertexArrayObject& operator=(const OpenGLVertexArrayObject&) = delete;

			OpenGLVertexArrayObject(OpenGLVertexArrayObject&& other) noexcept;
			OpenGLVertexArrayObject& operator=(OpenGLVertexArrayObject&& other) noexcept;

			virtual void SetupIntorprit(unsigned int location, unsigned int sizeArgument, unsigned int step, const ETypeData& typeData, const VertexBufferObject& bufferObject, const void* beginStep=0) override;
			
			virtual void DeleteVertexObject() override;

			virtual void Bind() const override;
			virtual void UnBind() const override;

		private:

			unsigned int m_VAO;
			bool m_isCreate;
		};
	}
}