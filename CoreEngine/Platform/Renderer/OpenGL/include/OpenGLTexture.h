#pragma once
#include <Render/includes/Texture.h>
#include <Core/includes/Base.h>
#include <glad/glad.h>
#include <stb_image.h>


namespace CoreEngine
{
	namespace Render
	{
		class OpenGLTexture2D : public Texture2D
		{
		public:

			OpenGLTexture2D(const char* path);
			~OpenGLTexture2D();

		public:

			virtual void SetTexParameter(const EParameterName& parameter, const EValueOfParameter& value) override;
			virtual void SetTexParameter(const EParamaterOfCustomValues& parameter, const float* value) override;

			virtual const char* GetPath() const override	{ return m_path.c_str(); }
			virtual unsigned int GetWidth() const override  { return m_width; }
			virtual unsigned int GetHeight() const override { return m_height; }

			virtual bool IsLoad() const override { return m_isLoad; }

			virtual void Bind(uint8_t layout=0) override;
			virtual void UnBind() override;

		private:

			static int GetFromEnumParameterName(const EParameterName& paramName);
			static int GetFromEnumParameterOfCustomValues(const EParamaterOfCustomValues& paramCustomValue);
			static int GetFromEnumValueOfParameter(const EValueOfParameter& valueOfParam);
			static int GetFromEnumChannel(const ETypeChannel& channel);
			static ETypeChannel GetEnumFromChannel(GLenum channel);

		private:

			unsigned int m_textureID;
			unsigned int m_width;
			unsigned int m_height;
			bool m_isLoad;
			String m_path;
			GLenum m_internalFormat;
			ETypeChannel m_channel;

		};
	}
}