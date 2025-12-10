#pragma once
#include "Core/includes/Base.h"
#include <Platform/Renderer/OpenGL/include/OpenGLFramebuffer.h>
#include <glad/glad.h>


namespace CoreEngine
{
	namespace Render
	{
		struct FramebufferArraySpecification : public FramebufferSpecification
		{
			uint32 LayerCount;
		};

		namespace OpenGL
		{
			class OpenGLFramebufferArray : public OpenGLFramebuffer
			{
			public:

				OpenGLFramebufferArray(const FramebufferArraySpecification& Spec);
				virtual void Reconstruct() override;
				virtual void ActivateDepthTexture() override;
				void BindDepthLayar(const uint32 Layer);

			private:

				uint32 CountLayer;
			};
		}

	}
}