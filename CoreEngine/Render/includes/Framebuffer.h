#pragma once
#include <Core/includes/Base.h>


namespace CoreEngine
{
	namespace Render
	{

		enum class EFramebufferTextureFormat : uint8
		{
			NONE = 0,

			RGBA8,
			DEPTH24_STENCIL8,
			RED_INTEGER
		};


		struct FramebufferTextureAttachment
		{
			FramebufferTextureAttachment(const EFramebufferTextureFormat& TexFormat) : Format(TexFormat)
			{ }


			EFramebufferTextureFormat Format;
		};

		struct FramebuffetAttacmentTextures
		{
			DArray<FramebufferTextureAttachment> Textures;
		};

		struct FramebufferSpecification
		{
			uint32 Width = 0;
			uint32 Height = 0;
			FramebuffetAttacmentTextures AttachTextures;
		};


		class Framebuffer
		{
		public:

			static SharedPtr<Framebuffer> Create(const FramebufferSpecification& Spec);

			virtual void Reconstruct() = 0;

			virtual uint32 GetColorAttachmentID(const int32 Index) const = 0;
			virtual int32 ReadPixel(const int32 IndexColorAttachment, const int32 X, const int32 Y) = 0;

			virtual void Resize(const uint32 Width, const uint32 Height) = 0;
			virtual const FramebufferSpecification& GetSpecifiction() const = 0;

			virtual void ClearTexture(const uint32 index) = 0;

			virtual void Bind() = 0;
			virtual void UnBind() = 0;

		};
	}
}