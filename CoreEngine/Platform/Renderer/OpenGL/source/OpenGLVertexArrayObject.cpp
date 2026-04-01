#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLConvertData.h>
#include <Core/includes/Engine.h>
#include <Render/includes/RenderCommand.h>

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
				DeleteVertexObject(Engine::Get()->GetRenderDevice().get());
			}

			OpenGLVertexArrayObject::OpenGLVertexArrayObject(OpenGLVertexArrayObject&& other) noexcept
			{
				this->operator=(std::move(other));
			}
			OpenGLVertexArrayObject& OpenGLVertexArrayObject::operator=(OpenGLVertexArrayObject&& other) noexcept
			{
				DeleteVertexObject(Engine::Get()->GetRenderDevice().get());

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
			void OpenGLVertexArrayObject::SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
														 const VertexBufferObject& bufferObject, const uint32 beginStep)
			{
				ASSERT("");
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
				Bind(Device);
				bufferObject.Bind(Device);
				if (typeData == ETypeData::INT || typeData == ETypeData::UNSIGNED_INT)
				{
					glVertexAttribIPointer(location, sizeArgument, GetAPITypeFromEnum(typeData), step * GetSizeOfFromEnum(typeData),
										   (GLvoid*)(beginStep * GetSizeOfFromEnum(typeData)));
				}
				else
				{
					glVertexAttribPointer(location, sizeArgument, GetAPITypeFromEnum(typeData), GL_FALSE, step * GetSizeOfFromEnum(typeData),
										  (GLvoid*)(beginStep * GetSizeOfFromEnum(typeData)));
				}
				glEnableVertexAttribArray(location);
				UnBind();
				bufferObject.UnBind();
			}

			void OpenGLVertexArrayObject::SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
														 const VertexBufferObject& bufferObject, const ElementBufferObject& elementObject,
														 const uint32 beginStep)
			{
				if (typeData == ETypeData::NONE)
				{
					EG_LOG(CORE, ELevelLog::WARNING, "Undefined type {0}", static_cast<int32>(typeData));
					return;
				}
				/*	if (!m_isCreate)
					{
						glGenVertexArrays(1, &m_VAO);
						m_isCreate = true;
					}*/
				Device->DeleteVAO(Handle);
				DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>> Attrib = {{location, sizeArgument, step, typeData, beginStep}};
				Handle = Device->CreateVAO(bufferObject.GetHandle(), elementObject.GetHandle(), Attrib);

				/*	Bind(Device);
					bufferObject.Bind(Device);
					elementObject.Bind();
					glVertexAttribPointer(location, sizeArgument, GetAPITypeFromEnum(typeData), GL_FALSE, step * GetSizeOfFromEnum(typeData),
										  (GLvoid*)(beginStep * GetSizeOfFromEnum(typeData)));
					glEnableVertexAttribArray(location);
					elementObject.UnBind();
					bufferObject.UnBind();
					UnBind();*/

				m_elementBuffer = &elementObject;
			}

			void OpenGLVertexArrayObject::SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
														 const uint32 beginStep)
			{
				ASSERT("");
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
				// Device->CreateVAO()

				// Bind(Device);
				glVertexAttribPointer(location, sizeArgument, GetAPITypeFromEnum(typeData), GL_FALSE, step * GetSizeOfFromEnum(typeData),
									  (GLvoid*)(beginStep * GetSizeOfFromEnum(typeData)));
				glEnableVertexAttribArray(location);
				UnBind();
			}

			void OpenGLVertexArrayObject::DeleteVertexObject(RenderDevice* Device)
			{
				Device->DeleteVAO(Handle);

				/*if (m_isCreate)
				{
					glDeleteVertexArrays(1, &m_VAO);
					m_isCreate = false;
				}*/
			}
			void OpenGLVertexArrayObject::Bind(RenderDevice* Device) const
			{
				/*if (m_elementBuffer)
				{
					m_elementBuffer->Bind();
				}*/
				// glBindVertexArray(m_VAO);
				Device->BindVAO(Handle);
			}
			void OpenGLVertexArrayObject::UnBind() const
			{
				/*if (m_elementBuffer)
				{
					m_elementBuffer->UnBind();
				}*/
				glBindVertexArray(0);
			}
			RHI::HandleVAO OpenGLVertexArrayObject::GetHandle() const
			{
				return Handle;
			}

			void OpenGLVertexArrayObject::ResetLinkElementBuffer()
			{
				m_elementBuffer = nullptr;
			}
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
