#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>


namespace CoreEngine
{
	namespace Render
	{

		enum class EFramebufferTextureFormat : uint8
		{
			NONE = 0,

			RGBA8,
			DEPTH24_STENCIL8,
			DEPTH_COMPONENT,
			RED_INTEGER,

			DEPTH = DEPTH24_STENCIL8
		};

		
		enum class EFramebufferTextureFilterConfig : uint8
		{
			LINER = 0,
			NEAREST
		};

		enum class EFramebufferTextureWrapConfig : uint8
		{
			CLAMP_TO_EDGE = 0,
			CLAMP_TO_BORDER,
			REPEAT
		};


		struct FramebufferTextureAttachment
		{
			FramebufferTextureAttachment(const EFramebufferTextureFormat& TexFormat, EFramebufferTextureFilterConfig Filter = EFramebufferTextureFilterConfig::NEAREST, 
				EFramebufferTextureWrapConfig Wrap = EFramebufferTextureWrapConfig::CLAMP_TO_EDGE) : Format(TexFormat), Filter(Filter), Wrap(Wrap)
			{ }

			FramebufferTextureAttachment(const FramebufferTextureAttachment& Other) 
			{
				Format = Other.Format;
				Filter = Other.Filter;
				Wrap = Other.Wrap;
				Color = Other.Color;
			}

			FramebufferTextureAttachment& operator=(const FramebufferTextureAttachment& Other)
			{
				Format = Other.Format;
				Filter = Other.Filter;
				Wrap = Other.Wrap;
				Color = Other.Color;

				return *this;
			}


			EFramebufferTextureFormat Format;
			EFramebufferTextureFilterConfig Filter = EFramebufferTextureFilterConfig::NEAREST;
			
			EFramebufferTextureWrapConfig Wrap = EFramebufferTextureWrapConfig::CLAMP_TO_EDGE;
			FVector4 Color;

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

			virtual ~Framebuffer() = default;

			static SharedPtr<Framebuffer> Create(const FramebufferSpecification& Spec);

			virtual void Reconstruct() = 0;

			virtual uint32 GetColorAttachmentID(const int32 Index) const = 0;
			virtual int32 ReadPixel(const int32 IndexColorAttachment, const int32 X, const int32 Y) = 0;

			virtual void Resize(const uint32 Width, const uint32 Height) = 0;
			virtual const FramebufferSpecification& GetSpecifiction() const = 0;

			virtual void ClearTexture(const uint32 index) = 0;
			virtual void ClearDepth() = 0;

			virtual void ActivateDepthTexture() = 0;
			virtual uint32 GetDepthAttachmentID() const = 0;

			virtual void Bind() = 0;
			virtual void UnBind() = 0;

		};
	}
}