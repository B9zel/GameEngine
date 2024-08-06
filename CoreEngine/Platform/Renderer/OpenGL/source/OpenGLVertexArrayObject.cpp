#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>


namespace CoreEngine
{	
	namespace Render
	{
		
		OpenGLVertexArrayObject::OpenGLVertexArrayObject()
		{
			m_VAO = 0;
			m_isCreate = true;
		}
		OpenGLVertexArrayObject::~OpenGLVertexArrayObject()
		{
			DeleteVertexObject();
		}
		OpenGLVertexArrayObject::OpenGLVertexArrayObject(OpenGLVertexArrayObject&& other) noexcept
		{
			this->operator=(std::move(other));
		}
		OpenGLVertexArrayObject& OpenGLVertexArrayObject::operator=(OpenGLVertexArrayObject&& other) noexcept
		{
			DeleteVertexObject();

			m_VAO = other.m_VAO;
			m_isCreate = other.m_isCreate;

			other.m_VAO = 0;
			other.m_isCreate = false;

			return *this;
		}
		void OpenGLVertexArrayObject::SetupIntorprit(unsigned int location, unsigned int sizeArgument, unsigned int step, const ETypeData& typeData, 
			const VertexBufferObject& bufferObject,const void* beginStep)
		{
			if (typeData == ETypeData::NONE)
			{
				EG_LOG(CORE, ELevelLog::WARNING, "Undefined type {0}", static_cast<int>(typeData));
				return;
			}
			if (!m_isCreate)
			{
				glGenVertexArrays(1, &m_VAO);
				m_isCreate = true;
			}
			bufferObject.Bind();
			Bind();
			glVertexAttribPointer(location, sizeArgument, GetAPITypeFromEnum(typeData),  GL_FALSE, step * GetSizeOfFromEnum(typeData), (GLvoid*)beginStep);
			glEnableVertexAttribArray(location);
			UnBind();
			bufferObject.UnBind();
		}
		void OpenGLVertexArrayObject::DeleteVertexObject()
		{
			if (m_isCreate)
			{
				glDeleteVertexArrays(1, &m_VAO);
				m_isCreate = false;
			}
		}
		void OpenGLVertexArrayObject::Bind() const
		{
			glBindVertexArray(m_VAO);
		}
		void OpenGLVertexArrayObject::UnBind() const
		{
			glBindVertexArray(0);
		}
	}
}