#include <Platform/Renderer/OpenGL/include/OpenGLElementBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLElementBufferObject::OpenGLElementBufferObject()
			{
				m_EBO = 0;
				m_isCreate = false;
				m_typeStorageData = ETypeData::NONE;
			}
			OpenGLElementBufferObject::~OpenGLElementBufferObject()
			{
				DeleteBuffer();
			}

			OpenGLElementBufferObject::OpenGLElementBufferObject(OpenGLElementBufferObject&& Other) noexcept
			{
				m_EBO = Other.m_EBO;
				m_isCreate = Other.m_isCreate;
				m_typeStorageData = Other.m_typeStorageData;

				Other.m_EBO = 0;
				Other.m_isCreate = false;
				Other.m_typeStorageData = ETypeData::NONE;
			}

			void OpenGLElementBufferObject::CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeDraw& typeDraw, const VertexArrayObject& vertexArray, const VertexBufferObject& buffObj)
			{
				if (IsCreate())
				{
					DeleteBuffer();
				}

				vertexArray.Bind();
				buffObj.Bind();
				glGenBuffers(1, &m_EBO);
				Bind();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSizeOfFromEnum(typeData) * sizeArr, dataArr, GetDrawTypeAPIFromEnum(typeDraw));
				vertexArray.UnBind();
				buffObj.UnBind();
				UnBind();

				m_typeStorageData = typeData;
				m_isCreate = true;
			}

			void OpenGLElementBufferObject::CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeDraw& typeDraw, const VertexArrayObject& vertexArray)
			{
				if (IsCreate())
				{
					DeleteBuffer();
				}

				vertexArray.Bind();
				glGenBuffers(1, &m_EBO);
				Bind();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSizeOfFromEnum(typeData) * sizeArr, dataArr, GetDrawTypeAPIFromEnum(typeDraw));
				vertexArray.UnBind();
				UnBind();
			
				m_typeStorageData = typeData;
				m_isCreate = true;
			}

			void OpenGLElementBufferObject::DeleteBuffer()
			{
				if (IsCreate())
				{
					glDeleteBuffers(1, &m_EBO);
					m_isCreate = false;
				}
			}
			bool OpenGLElementBufferObject::SetData(const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind)
			{
				if (!IsCreate()) return false;

#ifdef ENGINE_DEBUG

				int32 bufferSize;
				Bind();
				glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
				UnBind();
				if ((beginFillData + sizeData * GetSizeOfFromEnum(m_typeStorageData)) >= static_cast<uint32>(bufferSize))
				{
					EG_LOG(CORE, ELevelLog::WARNING, "Out of range element vertex buffer");
					return false;
				}

#endif // ENGINE_DEBUG

				if (isBind)
				{
					Bind();
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeData * GetSizeOfFromEnum(m_typeStorageData), data);
					UnBind();
				}
				else
				{
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeData * GetSizeOfFromEnum(m_typeStorageData), data);
				}

				return false;
			}
			void OpenGLElementBufferObject::Bind() const
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			}
			void OpenGLElementBufferObject::UnBind() const
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
	}
}