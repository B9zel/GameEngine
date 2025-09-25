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
				virtual uint32 GetColorAttachmentID() const override;
				virtual void Resize(const uint32 Width, const uint32 Height) override;
				virtual const FramebufferSpecification& GetSpecifiction() const override;

				virtual void Bind() override;
				virtual void UnBind() override;

			private:

				void ClearBuffer();

			private:

				uint32 m_FramebufferID{ 0 };
				uint32 m_ColorAttachTex{ 0 };
				uint32 m_DepthAttachTex{ 0 };
				FramebufferSpecification Specific;
			};
		}

	}
}