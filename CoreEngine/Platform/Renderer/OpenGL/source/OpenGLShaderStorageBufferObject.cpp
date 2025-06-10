#include <Platform/Renderer/OpenGL/include/OpenGLShaderStorageBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>

#include <glad/glad.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLShaderStorageBufferObject::OpenGLShaderStorageBufferObject()
			{
				m_BufferID = 0;
				m_IsCreated = false;
			}

			void OpenGLShaderStorageBufferObject::GenerationBuffer()
			{
				if (m_IsCreated) return;

				glGenBuffers(1, &m_BufferID);
				m_IsCreated = true;
			}

			void OpenGLShaderStorageBufferObject::CreaterBuffer(const void* DataArr, const uint32 sizeArr, const uint32 BytesOfData, const ETypeDraw& typeDraw)
			{
				if (m_IsCreated)
				{
					DeleteBuffer();
				}
				glGenBuffers(1, &m_BufferID);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeArr * BytesOfData, DataArr, GetDrawTypeAPIFromEnum(typeDraw));
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				m_IsCreated = true;
			}
			void OpenGLShaderStorageBufferObject::DeleteBuffer()
			{
				glDeleteBuffers(1, &m_BufferID);
			}
			bool OpenGLShaderStorageBufferObject::SetData(const int32 beginFillData, const uint32 sizeOfData, const uint32 BytesOfData, const void* data, const bool isBind)
			{
				if (!m_IsCreated) return false;

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
				glBufferSubData(GL_SHADER_STORAGE_BUFFER, BytesOfData * beginFillData, BytesOfData * sizeOfData, data);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				return true;
			}
			void OpenGLShaderStorageBufferObject::Bind(const uint32 IndexOfBuffer)
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, IndexOfBuffer, m_BufferID);

			}
			void OpenGLShaderStorageBufferObject::UnBind()
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}

		}
	}
}