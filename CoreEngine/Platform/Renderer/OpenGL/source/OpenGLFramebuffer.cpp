#include <Platform/Renderer/OpenGL/include/OpenGLFramebuffer.h>
#include <Core/includes/Base.h>
#include <glad/glad.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			static const uint32 MaxBufferSize = 8192;

			OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& Spec)
			{
				Specific = Spec;
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

				glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachTex);
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachTex);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Specific.Width, Specific.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachTex, 0);

				glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachTex);
				glBindTexture(GL_TEXTURE_2D, m_DepthAttachTex);
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Specific.Width, Specific.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachTex, 0);

				CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer doesn't complete");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			uint32 OpenGLFramebuffer::GetColorAttachmentID() const
			{
				return m_ColorAttachTex;
			}

			void OpenGLFramebuffer::Resize(const uint32 Width, const uint32 Height)
			{
				if (Width <= MaxBufferSize && Height <= MaxBufferSize)
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
				glDeleteTextures(1, &m_ColorAttachTex);
				glDeleteTextures(1, &m_DepthAttachTex);
			}
		}
	}
}