#include <Platform/Renderer/OpenGL/include/OpenGLFramebuffer.h>

#include <glad/glad.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			static const uint32 MaxBufferSize = 8192;

			static void AddColorAttach(const int32 IdOfColor, const GLint InternalFormat, const GLenum Format, const GLenum Type, const uint32 Width, const uint32 Height, const int32 Index = 0)
			{
				glBindTexture(GL_TEXTURE_2D, IdOfColor);

				glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, Format, Type, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Index, GL_TEXTURE_2D, IdOfColor, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			static void AddDepthAttach(const int32 IdOfColor, const GLenum Attachment, const GLenum Type, const uint32 Width, const uint32 Height)
			{
				glBindTexture(GL_TEXTURE_2D, IdOfColor);

				//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Width, Height, 0, GL_DEPTH_STENCIL, Type, NULL);
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Width, Height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, IdOfColor, 0);

				glBindTexture(GL_TEXTURE_2D, 0);
			}

			OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& Spec) : m_DepthAttachmentsSpec(EFramebufferTextureFormat::NONE)
			{
				Specific = Spec;
				for (auto& Attach : Spec.AttachTextures.Textures)
				{
					if (Attach.Format == EFramebufferTextureFormat::DEPTH24_STENCIL8)
					{
						CORE_UNASSERT(m_DepthAttachmentsSpec.Format != EFramebufferTextureFormat::NONE, "Framebuffer can't have more than one depth attachment");
						m_DepthAttachmentsSpec = Attach;
					}
					else
					{
						m_ColorAttachmentsSpec.push_back(Attach);
					}
				}
				Reconstruct();
			}
			void OpenGLFramebuffer::Reconstruct()
			{
				if (m_FramebufferID)
				{
					ClearBuffer();
				}

				glCreateFramebuffers(1, &m_FramebufferID);
				glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

				if (!m_ColorAttachmentsSpec.empty())
				{
					m_ColorAttachID.resize(m_ColorAttachmentsSpec.size());
					
					glCreateTextures(GL_TEXTURE_2D, m_ColorAttachmentsSpec.size(), m_ColorAttachID.data());
					for (int32 i = 0; i < m_ColorAttachmentsSpec.size(); ++i)
					{
						const auto& Spec = m_ColorAttachmentsSpec[i];

						switch (Spec.Format)
						{
						case EFramebufferTextureFormat::RGBA8:
							AddColorAttach(m_ColorAttachID[i], GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, Specific.Width, Specific.Height, i);
							break;
						case EFramebufferTextureFormat::RED_INTEGER:
							AddColorAttach(m_ColorAttachID[i], GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_INT, Specific.Width, Specific.Height, i);
							break;
						default:
							break;
						}
					}
				}

				if (m_DepthAttachmentsSpec.Format != EFramebufferTextureFormat::NONE)
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachID);
					AddDepthAttach(m_DepthAttachID, GL_DEPTH_STENCIL_ATTACHMENT, GL_UNSIGNED_INT_24_8, Specific.Width, Specific.Height);
				}

				if (m_ColorAttachID.size() > 1)
				{
					static DArray<GLenum> Attaches;
					Attaches.clear();
					for (uint32 i = 0; i < m_ColorAttachID.size(); i++)
					{
						Attaches.emplace_back(GL_COLOR_ATTACHMENT0 + i);
					}
					glDrawBuffers(Attaches.size(), Attaches.data());
				}
				else
				{
					glDrawBuffer(GL_NONE);
				}

				/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Specific.Width, Specific.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachTex, 0);

				glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachTex);
				glBindTexture(GL_TEXTURE_2D, m_DepthAttachTex);
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Specific.Width, Specific.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachTex, 0);*/

				CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer doesn't complete");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			uint32 OpenGLFramebuffer::GetColorAttachmentID(const int32 Index) const
			{
				if (m_ColorAttachID.empty())
				{
					EG_LOG(CoreEngine::CORE, ELevelLog::WARNING, "Framebuffer doesn't have color attachment");
					return 0;
				}

				return m_ColorAttachID[Index];
			}

			void OpenGLFramebuffer::Resize(const uint32 Width, const uint32 Height)
			{
				if (0 < Width &&  Width <= MaxBufferSize && 0 < Height && Height <= MaxBufferSize)
				{
					Specific.Width = Width;
					Specific.Height = Height;
					Reconstruct();
				}

			}

			const FramebufferSpecification& OpenGLFramebuffer::GetSpecifiction() const
			{
				return Specific;	
			}

			int32 OpenGLFramebuffer::ReadPixel(const int32 IndexColorAttachment, const int32 X, const int32 Y)
			{
				//Bind();

				glReadBuffer(GL_COLOR_ATTACHMENT0 + IndexColorAttachment);
				int32 PixelData = 0;
				glReadPixels(X, Y, 1, 1, GL_RED_INTEGER, GL_INT, &PixelData);

				//UnBind();

				return PixelData;
			}

			void OpenGLFramebuffer::ClearTexture(const uint32 index)
			{
				const GLuint clearValue = 0;
				glClearBufferuiv(GL_COLOR, index, &clearValue);
			}

			void OpenGLFramebuffer::Bind()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
				glViewport(0, 0, Specific.Width, Specific.Height);
			}

			void OpenGLFramebuffer::UnBind()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			void OpenGLFramebuffer::ClearBuffer()
			{
				glDeleteFramebuffers(1, &m_FramebufferID);
				glDeleteTextures(m_ColorAttachID.size(), m_ColorAttachID.data());
				glDeleteTextures(1, &m_DepthAttachID);

				m_ColorAttachID.clear();
				m_DepthAttachID = 0;
			}
		}
	}
}