#pragma once
#include "Core/includes/Base.h"
#include <Render/includes/Framebuffer.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{


			class OpenGLFramebuffer : public Framebuffer
			{
			public:

				OpenGLFramebuffer(const FramebufferSpecification& Spec);

			public:

				virtual void Reconstruct() override;
				virtual uint32 GetColorAttachmentID(const int32 Index) const override;
				virtual void Resize(const uint32 Width, const uint32 Height) override;
				virtual const FramebufferSpecification& GetSpecifiction() const override;
				virtual int32 ReadPixel(const int32 IndexColorAttachment, const int32 X, const int32 Y) override;
				virtual void ClearTexture(const uint32 index) override;

				virtual void Bind() override;
				virtual void UnBind() override;
				

			private:

				void ClearBuffer();

			private:

				uint32 m_FramebufferID{ 0 };

				DArray<FramebufferTextureAttachment> m_ColorAttachmentsSpec;
				FramebufferTextureAttachment m_DepthAttachmentsSpec;

				DArray<uint32> m_ColorAttachID;
				uint32 m_DepthAttachID;

				FramebufferSpecification Specific;
			};
		}

	}
}