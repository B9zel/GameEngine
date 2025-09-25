#pragma once
#include <Core/includes/Base.h>


namespace CoreEngine
{
	namespace Render
	{

		struct FramebufferSpecification
		{
			uint32 Width = 0;
			uint32 Height = 0;
		};

		class Framebuffer
		{
		public:

			static SharedPtr<Framebuffer> Create(const FramebufferSpecification& Spec);

			virtual void Reconstruct() = 0;
			virtual uint32 GetColorAttachmentID() const = 0;
			virtual void Resize(const uint32 Width, const uint32 Height) = 0;
			virtual const FramebufferSpecification& GetSpecifiction() const = 0;

			virtual void Bind() = 0;
			virtual void UnBind() = 0;

		};
	}
}