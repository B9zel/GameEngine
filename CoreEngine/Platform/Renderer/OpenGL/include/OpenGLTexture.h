#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Math.h>
#include <Render/includes/Texture.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <glad/glad.h>
#include <gl/GL.h>
#include <stb_image.h>

// #define STB_IMAGE_IMPLEMENTATION

namespace CoreEngine
{
	namespace Render
	{
		class RenderDevice;
		class Texture;
		class Texture2D;
		enum class ETextureWrap : uint8;
		enum class EValueOfParameter : uint8;
		enum class EParamaterOfCustomValues : uint8;

		namespace OpenGL
		{
			class OpenGLTexture2D : public Texture2D
			{
			public:

				OpenGLTexture2D(const OpenGLTexture2D&) = delete;
				OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

			private:

				// OpenGLTexture2D();
				//~OpenGLTexture2D();

			public:

				OpenGLTexture2D() = delete;

				OpenGLTexture2D(RenderDevice* Device, const String& path);
				OpenGLTexture2D(OpenGLTexture2D&& other) noexcept;
				OpenGLTexture2D& operator=(OpenGLTexture2D&& other) noexcept;

			public:

				virtual void SetTexParameter(const ETextureWrap parameter, const EValueOfParameter value) const override;
				virtual void SetTexParameter(const EParamaterOfCustomValues parameter, const float* value) const override;

				virtual void RecreateTexture(const uint32 Width, const uint32 Height, const ETypeChannel Channel, const void* Data,
											 bool isGenaretMipmap = true) override;

				virtual const char* GetPath() const override
				{
					return m_path.c_str();
				}
				virtual uint32 GetWidth() const override
				{
					return m_width;
				}
				virtual uint32 GetHeight() const override
				{
					return m_height;
				}

				virtual bool IsLoad() const override
				{
					return m_isLoad;
				}

				virtual void Bind(RenderDevice* Device, uint32 layout = 0) const override;
				virtual void UnBind() const override;

				virtual bool LoadTexture(RenderDevice* Device, const StringView Path, const bool GenerateMips = true);
				void SetLevelMipmap(const char* path, uint8 level = 1);

				virtual RHI::TextureHandle& GetTextureHandle() override;
				virtual const RHI::TextureHandle& GetConstTextureHandle() const override;

			public:

				static int32 GetInternalFormatFromEnum(const ETypeChannel& channel);
				static int32 GetWrapFromEnum(const ETextureWrap& paramName);
				static int32 GetFilterFromEnum(const ETextureFilter& Filter);
				static int32 GetFromEnumParameterOfCustomValues(const EParamaterOfCustomValues& paramCustomValue);
				static int32 GetFromEnumValueOfParameter(const EValueOfParameter& valueOfParam);
				static ETypeChannel GetEnumFromChannel(GLenum channel);

			private:

				void SetDefaultSettings();

			private:

				uint32 m_textureID;
				uint32 m_width;
				uint32 m_height;
				bool m_isLoad;
				String m_path;
				GLenum m_internalFormat;
				ETypeChannel m_channel;

				RHI::TextureHandle Handle;
				TextureDesc Desc;
			};
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
