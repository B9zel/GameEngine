#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/RenderHardwareInterface.h>
// #include <Core/includes/AssetManager.h>

namespace CoreEngine
{
	class AssetManager;

	namespace Render
	{
		class RenderDevice;

		enum class ETypeChannel : uint8
		{
			NONE = 0,
			RGB,
			RGBA
		};

		enum class ETextureWrap : uint8
		{
			//// Takes on the values : REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER
			// TEXTURE_WRAP_S = 0,
			// TEXTURE_WRAP_T,
			//// Use for 3D texture
			// TEXTURE_WRAP_R,

			// TEXTURE_MIN_FILTER,
			// TEXTURE_MAG_FILTER

			REPEAT = 0,
			MIRRORED_REPEAT,
			CLAMP_TO_EDGE
		};

		enum class ETextureFilter : uint8
		{
			NEAREST = 0,
			LINEAR
		};

		enum class EParamaterOfCustomValues : uint8
		{
			// Takes on the value: RGBA color array(float*)
			// Use if TEXTURE_WRAP_* used CLAMP_TO_BORDER
			TEXTURE_BORDER_COLOR,
		};

		enum class EValueOfParameter : uint8
		{
			// Repeat texure
			REPEAT = 0,
			// Repeat texture mirrored
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

		struct TextureDesc
		{
			int32 Width, Height, Channels;
			uint8* Data;

			ETextureFilter MinFilter = ETextureFilter::LINEAR;
			ETextureFilter MaxFilter = ETextureFilter::LINEAR;

			ETextureWrap WrapS = ETextureWrap::REPEAT;
			ETextureWrap WrapT = ETextureWrap::REPEAT;

			bool GenerateMips = true;
		};

		class Texture
		{
		public:

			virtual ~Texture() = default;

			virtual void SetTexParameter(const ETextureWrap parameter, const EValueOfParameter value) const = 0;
			virtual void SetTexParameter(const EParamaterOfCustomValues parameter, const float* value) const = 0;

			virtual void RecreateTexture(const uint32 Width, const uint32 Height, const ETypeChannel Channel, const void* Data,
										 bool isGenaretMipmap = true) = 0;

			virtual bool LoadTexture(RenderDevice* Device, const StringView Path, const bool GenerateMips = true) = 0;

			virtual const char* GetPath() const = 0;
			virtual uint32 GetWidth() const = 0;
			virtual uint32 GetHeight() const = 0;

			virtual bool IsLoad() const = 0;

			virtual void Bind(RenderDevice* Device, uint32 layout = 0) const = 0;
			virtual void UnBind() const = 0;

			virtual RHI::TextureHandle& GetTextureHandle() = 0;
			virtual const RHI::TextureHandle& GetConstTextureHandle() const = 0;
		};

		class Texture2D : public Texture
		{
		public:

			friend CoreEngine::AssetManager;

		protected:

			Texture2D() = default;

		private:

			static UniquePtr<Texture2D> Create(RenderDevice* Device, const String path);
			//static UniquePtr<Texture2D> Create();
		};
	} // namespace Render
} // namespace CoreEngine
