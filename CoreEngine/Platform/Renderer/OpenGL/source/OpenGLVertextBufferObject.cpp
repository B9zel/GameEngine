#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLVertexBufferObject::OpenGLVertexBufferObject()
			{
				m_VBO = 0;
				m_IsCreate = false;
				m_typeStorageData = ETypeData::NONE;
			}

			OpenGLVertexBufferObject::~OpenGLVertexBufferObject()
			{
				DeleteBuffer();
			}

			OpenGLVertexBufferObject::OpenGLVertexBufferObject(OpenGLVertexBufferObject&& other) noexcept
			{
				this->operator=(std::move(other));
			}

			OpenGLVertexBufferObject& OpenGLVertexBufferObject::operator=(OpenGLVertexBufferObject&& other) noexcept
			{
				DeleteBuffer();

				m_VBO = other.m_VBO;
				m_IsCreate = other.m_IsCreate;
				m_typeStorageData = other.m_typeStorageData;

				other.m_VBO = 0;
				other.m_IsCreate = false;
				other.m_typeStorageData = ETypeData::NONE;

				return *this;
			}

			void OpenGLVertexBufferObject::CreaterBuffer(const void* vertexArr, const uint32 sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw, const VertexArrayObject& vertexArray)
			{
				if (m_IsCreate) return;

				vertexArray.Bind();
				glGenBuffers(1, &m_VBO);

				Bind();
				glBufferData(GL_ARRAY_BUFFER, GetSizeOfFromEnum(typeArr) * sizeArr, vertexArr, GetDrawTypeAPIFromEnum(typeDraw));
				vertexArray.UnBind();
				UnBind();

				m_typeStorageData = typeArr;
				m_IsCreate = true;
			}

			void OpenGLVertexBufferObject::DeleteBuffer()
			{
				if (m_IsCreate)
				{
					glDeleteBuffers(1, &m_VBO);
					m_IsCreate = false;
				}
			}

			bool OpenGLVertexBufferObject::SetData(const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind)
			{
				if (!IsCreate() || m_typeStorageData == ETypeData::NONE) return false;
#ifdef ENGINE_DEBUG

				int bufferSize;
				Bind();
				glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
				UnBind();
				if ((beginFillData + sizeData * GetSizeOfFromEnum(m_typeStorageData)) >= static_cast<uint32>(bufferSize))
				{
					EG_LOG(CORE, ELevelLog::WARNING, "Out of range vertex buffer");
					return false;
				}

#endif // ENGINE_DEBUG

				if (isBind)
				{
					Bind();
					glBufferSubData(GL_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeData * GetSizeOfFromEnum(m_typeStorageData), data);
					UnBind();
				}
				else
				{
					glBufferSubData(GL_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeData * GetSizeOfFromEnum(m_typeStorageData), data);
				}
				return true;
			}



			void OpenGLVertexBufferObject::Bind() const
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			}

			void OpenGLVertexBufferObject::UnBind() const
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			bool OpenGLVertexBufferObject::IsCreate()
			{
				return m_IsCreate;
			}
		}
	}
}