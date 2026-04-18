#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/Enums/ShaderRenderType.h>
#include <Render/includes/ShaderUtils.h>

class ShaderAsset;
namespace CoreEngine::Render
{
	struct ShaderVariantKey;

	struct CompileShaderSource
	{
		String VertexShader;
		String FragmentShader;
	};

	class ShaderCompiler
	{
	private:

		ShaderCompiler();

	public:

		static ShaderCompiler& Get();
		CompileShaderSource Build(const ShaderAsset& Asset, const ShaderVariantKey& ShaderVarient);

	private:

		void Initialize();

		static HashTableMap<EShaderRenderType, SourceShader> TypeRenderShaders;

		static String VertexShaderKeyWord;
		static String FragmentShaderKeyWord;
	};
} // namespace CoreEngine::Render
