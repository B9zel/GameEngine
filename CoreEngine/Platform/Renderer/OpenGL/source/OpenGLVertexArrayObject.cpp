#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLVertexArrayObject::OpenGLVertexArrayObject()
			{
				m_VAO = 0;
				m_isCreate = false;
				m_elementBuffer = nullptr;
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
			void OpenGLVertexArrayObject::CreateVertexArray()
			{
				if (!m_isCreate)
				{
					glGenVertexArrays(1, &m_VAO);
					m_isCreate = true;
				}
			}
			void OpenGLVertexArrayObject::SetupIntorprit(uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
				const VertexBufferObject& bufferObject, const uint32 beginStep)
			{
				if (typeData == ETypeData::NONE)
				{
					EG_LOG(CORE, ELevelLog::WARNING, "Undefined type {0}", static_cast<int32>(typeData));
					return;
				}
				if (!m_isCreate)
				{
					glGenVertexArrays(1, &m_VAO);
					m_isCreate = true;
				}
				Bind();
				bufferObject.Bind();
				glVertexAttribPointer(location, sizeArgument, GetAPITypeFromEnum(typeData), GL_FALSE, step * GetSizeOfFromEnum(typeData), (GLvoid*)(beginStep * GetSizeOfFromEnum(typeData)));
				glEnableVertexAttribArray(location);
				UnBind();
				bufferObject.UnBind();
			}

			void OpenGLVertexArrayObject::SetupIntorprit(uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
				const VertexBufferObject& bufferObject, const ElementBufferObject& elementObject, const uint32 beginStep)
			{
				if (typeData == ETypeData::NONE)
				{
					EG_LOG(CORE, ELevelLog::WARNING, "Undefined type {0}", static_cast<int32>(typeData));
					return;
				}
				if (!m_isCreate)
				{
					glGenVertexArrays(1, &m_VAO);
					m_isCreate = true;
				}
				Bind();
				bufferObject.Bind();
				elementObject.Bind();
				glVertexAttribPointer(location, sizeArgument, GetAPITypeFromEnum(typeData), GL_FALSE, step * GetSizeOfFromEnum(typeData), (GLvoid*)(beginStep * GetSizeOfFromEnum(typeData)));
				glEnableVertexAttribArray(location);
				elementObject.UnBind();
				bufferObject.UnBind();
				UnBind();

				m_elementBuffer = &elementObject;
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
				/*if (m_elementBuffer)
				{
					m_elementBuffer->Bind();
				}*/
				glBindVertexArray(m_VAO);
			}
			void OpenGLVertexArrayObject::UnBind() const
			{
				/*if (m_elementBuffer)
				{
					m_elementBuffer->UnBind();
				}*/
				glBindVertexArray(0);
			}
			void OpenGLVertexArrayObject::ResetLinkElementBuffer()
			{
				m_elementBuffer = nullptr;
			}
		}
	}
}