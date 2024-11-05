#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Math.h>
#include <Render/includes/Texture.h>
#include <glad/glad.h>
#include <gl/GL.h>
#include <stb_image.h>

//#define STB_IMAGE_IMPLEMENTATION




namespace CoreEngine
{
	namespace Render
	{
		class Texture;
		class Texture2D;
		enum class EParameterName : uint8;
		enum class EValueOfParameter : uint8;
		enum class EParamaterOfCustomValues : uint8;


		namespace OpenGL
		{
			class OpenGLTexture2D : public Texture2D
			{
			public:

				OpenGLTexture2D(const OpenGLTexture2D&) = delete;
				OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

				OpenGLTexture2D();
				~OpenGLTexture2D();

				OpenGLTexture2D(const char* path);
				OpenGLTexture2D(OpenGLTexture2D&& other) noexcept;

				OpenGLTexture2D& operator=(OpenGLTexture2D&& other) noexcept;

			public:

				virtual void SetTexParameter(const EParameterName parameter, const EValueOfParameter value) const override;
				virtual void SetTexParameter(const EParamaterOfCustomValues parameter, const float* value) const override;

				virtual const char* GetPath() const override { return m_path.c_str(); }
				virtual uint32 GetWidth() const override { return m_width; }
				virtual uint32 GetHeight() const override { return m_height; }

				virtual bool IsLoad() const override { return m_isLoad; }

				virtual void Bind(uint32 layout = 0) const override;
				virtual void UnBind() const override;

				bool ChangeTexture(const char* path, bool isGenaretMipmap = true);
				void SetLevelMipmap(const char* path, uint8_t level = 1);
			private:

				void SetDefaultSettings();

			private:

				static int32 GetFromEnumParameterName(const EParameterName& paramName);
				static int32 GetFromEnumParameterOfCustomValues(const EParamaterOfCustomValues& paramCustomValue);
				static int32 GetFromEnumValueOfParameter(const EValueOfParameter& valueOfParam);
				static int32 GetFromEnumChannel(const ETypeChannel& channel);
				static ETypeChannel GetEnumFromChannel(GLenum channel);

			private:

				uint32 m_textureID;
				uint32 m_width;
				uint32 m_height;
				bool m_isLoad;
				String m_path;
				GLenum m_internalFormat;
				ETypeChannel m_channel;
			};
		}
	}
}