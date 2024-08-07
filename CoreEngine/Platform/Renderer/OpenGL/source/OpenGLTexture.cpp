#include "../include/OpenGLTexture.h"



namespace CoreEngine
{
	namespace Render
	{
		int CoreEngine::Render::OpenGLTexture2D::GetFromEnumParameterName(const EParameterName& paramName)
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

		int CoreEngine::Render::OpenGLTexture2D::GetFromEnumParameterOfCustomValues(const EParamaterOfCustomValues& paramCustomValue)
		{
			switch (paramCustomValue)
			{
			case EParamaterOfCustomValues::TEXTURE_BORDER_COLOR: return GL_TEXTURE_BORDER_COLOR;
			default:
				throw std::exception("Unable to define EParamaterOfCustomValues enum");
			}
		}

		int CoreEngine::Render::OpenGLTexture2D::GetFromEnumValueOfParameter(const EValueOfParameter& valueOfParam)
		{
			switch (valueOfParam)
			{
			case EValueOfParameter::NEAREST:					return GL_NEAREST;
			case EValueOfParameter::LINEAR:						return GL_LINEAR;
			case EValueOfParameter::REPEAT:						return GL_REPEAT;
			case EValueOfParameter::MIRRORED_REPEAT:			return GL_MIRRORED_REPEAT;
			case EValueOfParameter::CLAMP_TO_BORDER:			return GL_CLAMP_TO_BORDER;
			case EValueOfParameter::CLAMP_TO_EDGE:				return GL_CLAMP_TO_EDGE;
			case EValueOfParameter::LINEAR_MIPMAP_LINEAR:		return GL_LINEAR_MIPMAP_LINEAR;
			case EValueOfParameter::LINEAR_MIPMAP_NEAREST:		return GL_LINEAR_MIPMAP_NEAREST;
			case EValueOfParameter::NEAREST_MIPMAP_NEAREST:		return GL_NEAREST_MIPMAP_NEAREST;
			case EValueOfParameter::NEAREST_MIPMAP_LINEAR:		return GL_NEAREST_MIPMAP_LINEAR;
			default:
				throw std::exception("Unable to define EValueOfParameter enum");
			}
		}

		int CoreEngine::Render::OpenGLTexture2D::GetFromEnumChannel(const ETypeChannel& channel)
		{
			switch (channel)
			{
			case ETypeChannel::RGB: return GL_RGB;
			case ETypeChannel::RGBA: return GL_RGBA;
			default:
				throw std::exception("Unable to define ETypeChannel enum");
			}
		}

		ETypeChannel CoreEngine::Render::OpenGLTexture2D::GetEnumFromChannel(GLenum channel)
		{
			switch (channel)
			{
			case GL_RGB: return ETypeChannel::RGB;
			case GL_RGBA: return ETypeChannel::RGBA;
			default:
				throw std::exception("Unable to define type of channe");
			}
		}


		CoreEngine::Render::OpenGLTexture2D::OpenGLTexture2D(const char* path) : m_textureID{ 0 }, m_width { 0 }, m_height{ 0 }, m_isLoad{false}, 
															 m_path(""), m_internalFormat(0), m_channel(ETypeChannel::NONE)
		{
			if (ChangeTexture(path))
			{
				Bind();

				SetTexParameter(EParameterName::TEXTURE_WRAP_S, EValueOfParameter::REPEAT);
				SetTexParameter(EParameterName::TEXTURE_WRAP_T, EValueOfParameter::REPEAT);
				SetTexParameter(EParameterName::TEXTURE_MIN_FILTER, EValueOfParameter::LINEAR);
				SetTexParameter(EParameterName::TEXTURE_MAG_FILTER, EValueOfParameter::LINEAR);

				UnBind();
			}
			
		}

		CoreEngine::Render::OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& other) noexcept
		{
			this->operator=(std::move(other));
		}

		CoreEngine::Render::OpenGLTexture2D::~OpenGLTexture2D()
		{
			glDeleteTextures(1, &m_textureID);
		}

		OpenGLTexture2D& CoreEngine::Render::OpenGLTexture2D::operator=(OpenGLTexture2D&& other) noexcept
		{
			glDeleteTextures(1, &m_textureID);
			m_textureID = other.m_textureID;

			m_height = other.m_height;
			m_width = other.m_width;

			m_channel = other.m_channel;
			m_internalFormat = other.m_internalFormat;

			m_isLoad = m_isLoad;
			m_path = std::move(other.m_path);

			other.m_width = other.m_height = 0;
			other.m_channel = ETypeChannel::NONE;
			other.m_internalFormat = 0;
			other.m_isLoad = false;
			other.m_textureID = 0;

			return *this;
		}

		void CoreEngine::Render::OpenGLTexture2D::SetTexParameter(const EParameterName parameter, const EValueOfParameter value) const
		{
			glTexParameteri(GL_TEXTURE_2D, GetFromEnumParameterName(parameter), GetFromEnumValueOfParameter(value));
		}
		
		void CoreEngine::Render::OpenGLTexture2D::SetTexParameter(const EParamaterOfCustomValues parameter, const float* value) const
		{
			glTexParameterfv(GL_TEXTURE_2D, GetFromEnumParameterOfCustomValues(parameter), value);
		}
		
		void CoreEngine::Render::OpenGLTexture2D::Bind(unsigned int layer) const
		{
			glActiveTexture(GL_TEXTURE0 + layer);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}

		void CoreEngine::Render::OpenGLTexture2D::UnBind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		bool CoreEngine::Render::OpenGLTexture2D::ChangeTexture(const char* path, bool isGenaretMipmap)
		{
			glDeleteTextures(1, &m_textureID);
			stbi_set_flip_vertically_on_load(1);

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
				if (isGenaretMipmap)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}

				UnBind();
				stbi_image_free(data);

				return true;
			}
			EG_LOG(CORE, ELevelLog::WARNING, "Can't load image {0}", path);
			return false;
		}
		void CoreEngine::Render::OpenGLTexture2D::SetLevelMipmap(const char* path, uint8_t level)
		{
			if (!IsLoad())
			{
				throw std::exception("Does't create texture");
			}
			stbi_uc* data = nullptr;
			int width, height, channel;
			data = stbi_load(path, &width, &height, &channel, 0);

			if (data)
			{
				if (channel == 3)
				{
					CORE_ASSERT(m_channel != ETypeChannel::RGB, "RGB texture cannot have RGBA mipmap");
				}
				else if (channel == 4)
				{
					CORE_ASSERT(m_channel != ETypeChannel::RGBA, "RGBA texture cannot have RGB mipmap");
				}
				
				Bind();
				glTexImage2D(GL_TEXTURE_2D, level, GetFromEnumChannel(m_channel), GetWidth(), GetHeight(), 0, GetFromEnumChannel(m_channel), GL_UNSIGNED_BYTE, data);
				UnBind();

				stbi_image_free(data);
			}
		}
	}
}
