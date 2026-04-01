#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Render/includes/RenderDevice.h>

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

			void OpenGLVertexBufferObject::CreateBuffer(RenderDevice* Device, const void* vertexArr, const uint32 sizeArr, const ETypeData& typeArr,
														const ETypeStorageDraw& typeDraw, const VertexArrayObject& vertexArray, const bool IsAutoUnBind)
			{
				if (m_IsCreate)
				{
					Device->DeleteVBO(m_Handle);
				}

				vertexArray.Bind(Device);
			/*	glGenBuffers(1, &m_VBO);

				Bind();
				glBufferData(GL_ARRAY_BUFFER, GetSizeOfFromEnum(typeArr) * sizeArr, vertexArr, GetDrawTypeAPIFromEnum(typeDraw));
				if (IsAutoUnBind)
				{
					vertexArray.UnBind();
					UnBind();
				}*/
				m_Handle = Device->CreateBuffer(EBufferTargetType::VERTEX_BUFFER, vertexArr, sizeArr, typeArr, typeDraw);

				m_typeStorageData = typeArr;
				m_IsCreate = true;
			}

			void OpenGLVertexBufferObject::CreateBuffer(RenderDevice* Device, const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr,
														const ETypeStorageDraw& typeDraw, const bool IsAutoUnBind)
			{
				if (m_IsCreate)
				{
					Device->DeleteVBO(m_Handle);
				}

				/*glGenBuffers(1, &m_VBO);

				Bind();
				glBufferData(GL_ARRAY_BUFFER, GetSizeOfFromEnum(typeArr) * sizeArr, vertexArr, GetDrawTypeAPIFromEnum(typeDraw));
				if (IsAutoUnBind)
				{
					UnBind();
				}*/

				m_Handle = Device->CreateBuffer(EBufferTargetType::VERTEX_BUFFER, vertexArr, sizeArr, typeArr, typeDraw);
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

			bool OpenGLVertexBufferObject::SetData(RenderDevice* Device, const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind)
			{
				if (!IsCreate() || m_typeStorageData == ETypeData::NONE) return false;
#ifdef ENGINE_DEBUG

				int bufferSize;
				Bind(Device);
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
					Bind(Device);
					glBufferSubData(GL_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeData * GetSizeOfFromEnum(m_typeStorageData),
									data);
					UnBind();
				}
				else
				{
					glBufferSubData(GL_ARRAY_BUFFER, beginFillData * GetSizeOfFromEnum(m_typeStorageData), sizeData * GetSizeOfFromEnum(m_typeStorageData),
									data);
				}
				return true;
			}

			void OpenGLVertexBufferObject::Bind(RenderDevice* Device) const
			{
				//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
				Device->BindVBO(m_Handle);
			}

			void OpenGLVertexBufferObject::UnBind() const
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			RHI::BufferHandle OpenGLVertexBufferObject::GetHandle() const
			{
				return m_Handle;
			}

			bool OpenGLVertexBufferObject::IsCreate()
			{
				return m_IsCreate;
			}
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
