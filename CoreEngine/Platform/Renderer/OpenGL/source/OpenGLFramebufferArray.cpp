#include <Platform/Renderer/OpenGL/include/OpenGLFramebufferArray.h>


namespace CoreEngine::Render::OpenGL
{
	OpenGLFramebufferArray::OpenGLFramebufferArray(const FramebufferArraySpecification& Spec) : OpenGLFramebuffer(Spec)
	{
		CountLayer = Spec.LayerCount;
	}

	static GLint ConverFromEnumToGLFilter(const EFramebufferTextureFilterConfig& Filter)
	{
		switch (Filter)
		{
		case EFramebufferTextureFilterConfig::LINER:
			return GL_LINEAR;
		case EFramebufferTextureFilterConfig::NEAREST:
			return GL_NEAREST;
		default:
			break;
		}
	}

	static GLint ConverFromEnumToWrap(const EFramebufferTextureWrapConfig& Wrap)
	{
		switch (Wrap)
		{
		case EFramebufferTextureWrapConfig::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case EFramebufferTextureWrapConfig::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		case EFramebufferTextureWrapConfig::REPEAT:
			return GL_REPEAT;
		default:
			break;
		}
	}

	static void AddColorAttach(const int32 IdOfColor, const GLint InternalFormat, const GLenum Format, const GLenum Type, const FramebufferTextureAttachment& TexConfig, const uint32 Width, const uint32 Height, const int32 Index, const uint32 CountLayer)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, IdOfColor);

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, InternalFormat, Width, Height, CountLayer, 0, Type, GL_FLOAT, nullptr);
	

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, ConverFromEnumToGLFilter(TexConfig.Filter));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, ConverFromEnumToGLFilter(TexConfig.Filter));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, ConverFromEnumToWrap(TexConfig.Wrap));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, ConverFromEnumToWrap(TexConfig.Wrap));

		if (TexConfig.Wrap == EFramebufferTextureWrapConfig::CLAMP_TO_BORDER)
		{
			float Color[4] = { TexConfig.Color.GetX(), TexConfig.Color.GetY(), TexConfig.Color.GetZ(), TexConfig.Color.GetW() };
			glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, Color);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Index, GL_TEXTURE_2D_ARRAY, IdOfColor, 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
	static void AddDepthAttach(const int32 IdOfColor, const GLint InternalForamat, const GLenum Type, const FramebufferTextureAttachment& TexConfig, const uint32 Width, const uint32 Height, const uint32 CountLayer)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, IdOfColor);



		//glTexImage2D(GL_TEXTURE_2D, 0, Type, Width, Height, 0, Type, GL_FLOAT, NULL);

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, InternalForamat, Width, Height, CountLayer, 0, Type, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, ConverFromEnumToGLFilter(TexConfig.Filter));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, ConverFromEnumToGLFilter(TexConfig.Filter));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, ConverFromEnumToWrap(TexConfig.Wrap));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, ConverFromEnumToWrap(TexConfig.Wrap));

		if (TexConfig.Wrap == EFramebufferTextureWrapConfig::CLAMP_TO_BORDER)
		{
			float Color[4] = { TexConfig.Color.GetX(), TexConfig.Color.GetY(), TexConfig.Color.GetZ(), TexConfig.Color.GetW() };
			glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, Color);
		}

		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, IdOfColor, 0, 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D_ARRAY, IdOfColor, 0);
	}

	void OpenGLFramebufferArray::Reconstruct()
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
					//AddColorAttach(m_ColorAttachID[i], GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, Spec, Specific.Width, Specific.Height, i);
					break;
				case EFramebufferTextureFormat::RED_INTEGER:
					//AddColorAttach(m_ColorAttachID[i], GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_INT, Spec, Specific.Width, Specific.Height, i);
					break;
				default:
					break;
				}
			}
		}

		if (m_DepthAttachmentsSpec.Format == EFramebufferTextureFormat::DEPTH24_STENCIL8)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachID);
			//AddDepthAttach(m_DepthAttachID, GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8, m_DepthAttachmentsSpec, Specific.Width, Specific.Height);
		}
		else if (m_DepthAttachmentsSpec.Format == EFramebufferTextureFormat::DEPTH_COMPONENT)
		{
			//glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachID);
			glGenTextures(1, &m_DepthAttachID);
			AddDepthAttach(m_DepthAttachID, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, m_DepthAttachmentsSpec, Specific.Width, Specific.Height, CountLayer);
		}

		if (m_ColorAttachID.size() >= 1)
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
			glReadBuffer(GL_NONE);
		}

		/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Specific.Width, Specific.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachTex, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachTex);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachTex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Specific.Width, Specific.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachTex, 0);*/
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			ASSERT("Framebuffer doesn't complete, status " + ToString(status));
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebufferArray::ActivateDepthTexture()
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_DepthAttachID);
	}
	void OpenGLFramebufferArray::BindDepthLayar(const uint32 Layer)
	{
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachID, 0, Layer);
	}
}
