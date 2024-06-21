#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>


namespace CoreEngine
{
	namespace Render
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

		void OpenGLVertexBufferObject::CreaterBuffer(const void* vertexArr, unsigned int sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw)
		{
			if (m_IsCreate) return;

			glGenBuffers(1, &m_VBO);
			m_typeStorageData = typeArr;
			Bind();
			glBufferData(GL_ARRAY_BUFFER, GetSizeOfFromEnum(typeArr) * sizeArr, vertexArr, GetDrawTypeAPIFromEnum(typeDraw));
			UnBind();

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

		void OpenGLVertexBufferObject::ChangeData(const int beginFillData,const unsigned int sizeOfData, const void* data, const bool isBind)
		{
			if (!m_IsCreate) return;
#ifdef ENGINE_DEBUG

			int bufferSize;
			Bind();
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
			UnBind();
			if ((beginFillData + sizeOfData * GetSizeOfFromEnum(m_typeStorageData)) > static_cast<unsigned>(bufferSize))
			{
				EG_LOG(CORE, ELevelLog::WARNING, "Out of range vertex buffer");
				return;
			}

#endif // ENGINE_DEBUG

			if (isBind)
			{
				Bind();
				glBufferSubData(GL_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeOfData, data);
				UnBind();
			}
			else
			{
				glBufferSubData(GL_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeOfData, data);
			}
		}

		bool OpenGLVertexBufferObject::SetData(const void* vertexArr, unsigned int sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw)
		{
			if (!m_IsCreate || typeArr == ETypeData::NONE) return false;
			
			m_typeStorageData = typeArr;
			Bind();
			glBufferData(GL_ARRAY_BUFFER, GetSizeOfFromEnum(typeArr) * sizeArr, vertexArr, GetDrawTypeAPIFromEnum(typeDraw));
			UnBind();

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