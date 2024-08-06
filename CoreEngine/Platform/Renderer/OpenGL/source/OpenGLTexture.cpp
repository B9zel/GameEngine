#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>
#include <Math/includes/Math.h>


namespace CoreEngine
{
	namespace Render
	{

		int OpenGLTexture2D::GetFromEnumParameterName(const EParameterName& paramName)
		{
			switch (paramName)
			{
			case EParameterName::TEXTURE_MAG_FILTER:	return GL_TEXTURE_MAG_FILTER;
			case EParameterName::TEXTURE_MIN_FILTER:	return GL_TEXTURE_MIN_FILTER;
			case EParameterName::TEXTURE_WRAP_R:		return GL_TEXTURE_WRAP_R;
			case EParameterName::TEXTURE_WRAP_S:		return GL_TEXTURE_WRAP_S;
			case EParameterName::TEXTURE_WRAP_T:		return GL_TEXTURE_WRAP_T;
			default:
				throw std::exception("Unable to define EParameterName enum");
			}
		}

		int OpenGLTexture2D::GetFromEnumParameterOfCustomValues(const EParamaterOfCustomValues& paramCustomValue)
		{
			switch (paramCustomValue)
			{
			case EParamaterOfCustomValues::TEXTURE_BORDER_COLOR: return GL_TEXTURE_BORDER_COLOR;
			default:
				throw std::exception("Unable to define EParamaterOfCustomValues enum");
			}
		}

		int OpenGLTexture2D::GetFromEnumValueOfParameter(const EValueOfParameter& valueOfParam)
		{
			switch (valueOfParam)
			{
			case EValueOfParameter::NEAREST:					return GL_NEAREST;
			case EValueOfParameter::LINEAR:					return GL_LINEAR;
			case EValueOfParameter::REPEAT:					return GL_REPEAT;
			case EValueOfParameter::MIRRORED_REPEAT:			return GL_MIRRORED_REPEAT;
			case EValueOfParameter::CLAMP_TO_BORDER:			return GL_CLAMP_TO_BORDER;
			case EValueOfParameter::CLAMP_TO_EDGE:				return GL_CLAMP_TO_EDGE;
			case EValueOfParameter::LINEAR_MIPMAP_LINEAR:		return GL_LINEAR_MIPMAP_LINEAR;
			case EValueOfParameter::LINEAR_MIPMAP_NEAREST:		return GL_LINEAR_MIPMAP_NEAREST;
			case EValueOfParameter::NEAREST_MIPMAP_NEAREST:	return GL_NEAREST_MIPMAP_NEAREST;
			case EValueOfParameter::NEAREST_MIPMAP_LINEAR:		return GL_NEAREST_MIPMAP_LINEAR;
			default:
				throw std::exception("Unable to define EValueOfParameter enum");
			}
		}

		int OpenGLTexture2D::GetFromEnumChannel(const ETypeChannel& channel)
		{
			switch (channel)
			{
			case ETypeChannel::RGB: return GL_RGB;
			case ETypeChannel::RGBA: return GL_RGBA;
			default:
				throw std::exception("Unable to define ETypeChannel enum");
			}
		}

		ETypeChannel OpenGLTexture2D::GetEnumFromChannel(GLenum channel)
		{
			switch (channel)
			{
			case GL_RGB: return ETypeChannel::RGB;
			case GL_RGBA: return ETypeChannel::RGBA;
			default:
				throw std::exception("Unable to define type of channe");
			}
		}




		OpenGLTexture2D::OpenGLTexture2D(const char* path) : m_textureID{ 0 }, m_width { 0 }, m_height{ 0 }, m_isLoad{false}, m_path("")
		{
			stbi_uc* data = nullptr;

			int width, height, channel;

			data = stbi_load(path, &width, &height, &channel, 0);
			if (data)
			{
				m_height = height;
				m_width = width;
				m_isLoad = true;
				m_path = path;

				if (channel == 3)
				{
					m_channel = ETypeChannel::RGB;
					m_internalFormat = GL_RGB8;
				}
				else if (channel == 4)
				{
					m_channel = ETypeChannel::RGBA;
					m_internalFormat = GL_RGBA8;
				}

				glGenTextures(1, &m_textureID);
				Bind();
				glTexImage2D(GL_TEXTURE_2D, 0, GetFromEnumChannel(m_channel), GetWidth(), GetHeight(), 0, GetFromEnumChannel(m_channel), GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				SetTexParameter(EParameterName::TEXTURE_WRAP_S, EValueOfParameter::REPEAT);
				SetTexParameter(EParameterName::TEXTURE_WRAP_T, EValueOfParameter::REPEAT);
				SetTexParameter(EParameterName::TEXTURE_MIN_FILTER, EValueOfParameter::LINEAR);
				SetTexParameter(EParameterName::TEXTURE_MAG_FILTER, EValueOfParameter::LINEAR);

				UnBind();

				stbi_image_free(data);
			}
		}
		OpenGLTexture2D::~OpenGLTexture2D()
		{
			glDeleteTextures(1, &m_textureID);
		}

		void OpenGLTexture2D::SetTexParameter(const EParameterName& parameter, const EValueOfParameter& value)
		{
			glTexParameteri(GL_TEXTURE_2D, GetFromEnumParameterName(parameter), GetFromEnumValueOfParameter(value));
		}
		void OpenGLTexture2D::SetTexParameter(const EParamaterOfCustomValues& parameter, const float* value)
		{
			glTexParameterfv(GL_TEXTURE_2D, GetFromEnumParameterOfCustomValues(parameter), value);
		}
		
		void OpenGLTexture2D::Bind(uint8_t layer)
		{
			glActiveTexture(Math::Clamp<uint8_t>(layer, 0, 16));
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}

		void OpenGLTexture2D::UnBind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}
