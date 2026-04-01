#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Math/includes/Matrix.h>
// #include <Render/includes/ShaderUtils.h>
//  #include <Core/includes/AssetManager.h>

namespace CoreEngine
{
	class AssetManager;

	namespace Render
	{
		class RenderDevice;

		class Shader
		{
		public:

			friend CoreEngine::AssetManager;

		public:

			static const String VertexShaderPoint;
			static const String FragmentShaderPoint;

		public:

			virtual ~Shader() = default;

			virtual bool CompileShader(RenderDevice* Device, const String& vertexShader, const String& fragmentShader) = 0;
			virtual bool CompileShader(RenderDevice* Device, const StringView vertexShader, const StringView fragmentShader) = 0;

			virtual bool GetIsCompile() = 0;
			virtual bool GetHasAllMatrix() = 0;
			virtual RHI::ShaderHandle GetHandle() const = 0;

			virtual void Bind(RenderDevice* Device) const = 0;
			virtual void UnBind(RenderDevice* Device) const = 0;

			virtual bool SetUniformMatrix4x4(RenderDevice* Device, const String& nameParam, const FMatrix4x4& matrix, bool isBindShader = true) = 0;
			virtual bool SetUniform1i(RenderDevice* Device, const String& nameParam, const int32 value, bool isEnableBind = true) = 0;
			virtual bool SetUniform1ui(RenderDevice* Device, const String& nameParam, const uint32 value, bool isEnableBind = true) = 0;
			virtual bool SetUniformFloat(RenderDevice* Device, const String& nameParam, float value, bool isEnableBind = true) = 0;
			virtual bool SetUniformVec4(RenderDevice* Device, const String& nameParam, const FVector4& vec, bool isEnableBind = true) = 0;
			virtual bool SetUniformVec2(RenderDevice* Device, const String& nameParam, const FVector2& vec, bool isEnableBind = true) = 0;
			virtual bool SetUniformVec3(RenderDevice* Device, const String& nameParam, const FVector& vec, bool isEnableBind = true) = 0;
			virtual const DArray<String>& GetNamesOfTexture() const = 0;

		private:

			/*
			 * @return First string store the vertex shader
			 * Second string store the fragment shader
			 */
			// static Pair<String, String> LoadShader(const char* pathToShader);
			static UniquePtr<Shader> CreateShader();
		};

	} // namespace Render
} // namespace CoreEngine
