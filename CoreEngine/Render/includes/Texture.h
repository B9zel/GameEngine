#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine
{
	namespace Render
	{		
		enum class ETypeChannel : uint8_t
		{
			NONE = 0,
			RGB,
			RGBA
		};

		enum class EParameterName : uint8_t
		{
			// Takes on the values : REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER
			TEXTURE_WRAP_S = 0, 
			TEXTURE_WRAP_T,
			// Use for 3D texture
			TEXTURE_WRAP_R,
		
			TEXTURE_MIN_FILTER,
			TEXTURE_MAG_FILTER
		};

		enum class EParamaterOfCustomValues : uint8_t
		{
			// Takes on the value: RGBA color array(float*)
			// Use if TEXTURE_WRAP_* used CLAMP_TO_BORDER
			TEXTURE_BORDER_COLOR,
		};

		enum class EValueOfParameter : uint8_t
		{
			// Repeat texure
			REPEAT = 0,
			//Repeat texture mirrored
			MIRRORED_REPEAT,
			// Takes color of border pixel and fill the free space
			CLAMP_TO_EDGE,
			// The same as CLAMP_TO_EDGE, but fill the free space custom color
			CLAMP_TO_BORDER,
			// Takes the nearest pixel
			NEAREST,
			// Takes the nearest pixels and interpolates
			LINEAR,
			// Selects the nearest mipmap corresponding to the pixel size, and also uses nearest neighbor interpolation for texture sampling
			NEAREST_MIPMAP_NEAREST,
			// Selects the nearest mipmap and samples it using linear interpolation
			LINEAR_MIPMAP_NEAREST,
			// Linear interpolation between two closest mipmaps and texture sampling using linear interpolation
			NEAREST_MIPMAP_LINEAR,
			// Gives you linear filtering on the two nearest mipmaps, and then does a linear blend between those two mipmaps
			LINEAR_MIPMAP_LINEAR
		};


		class Texture
		{
		public:

			virtual ~Texture() = default;

			virtual void SetTexParameter(const EParameterName parameter, const EValueOfParameter value) const = 0;
			virtual void SetTexParameter(const EParamaterOfCustomValues parameter, const float* value) const = 0;
			
			virtual const char* GetPath() const = 0;
			virtual unsigned int GetWidth() const = 0;
			virtual unsigned int GetHeight() const = 0;

			virtual bool IsLoad() const = 0;

			virtual void Bind(unsigned int layout=0) const = 0;
			virtual void UnBind() const = 0;

		};

		class Texture2D : public Texture
		{
		public:

			static SharedPtr<Texture2D> Create(const char* path);
		};
	}
}