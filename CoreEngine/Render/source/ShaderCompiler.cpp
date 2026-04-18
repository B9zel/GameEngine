#include <Render/includes/ShaderCompiler.h>
#include <Core/includes/AssetManager.h>
#include <Core/includes/Application.h>
#include <Core/includes/FileManager.h>
#include <Render/includes/ShaderVariantKey.h>
#include <Render/includes/ShaderAsset.h>

namespace CoreEngine::Render
{
	DECLARE_LOG_CATEGORY_EXTERN(ShaderCompilerLog);

	HashTableMap<EShaderRenderType, SourceShader> ShaderCompiler::TypeRenderShaders;
	String ShaderCompiler::VertexShaderKeyWord = "[[VERTEX]]";
	String ShaderCompiler::FragmentShaderKeyWord = "[[FRAGMENT]]";

	ShaderCompiler::ShaderCompiler()
	{
		Initialize();
	}

	void ShaderCompiler::Initialize()
	{
		SourceShader& Shader =
			AssetManager::Get().LoadStringShaderFromFile(Application::Get()->GetAppOptions().pathToProject + "/Shaders/StaticMeshBaseShader.glsl");

		if (!Shader.FragmentShader.empty() && !Shader.VertexShader.empty())
		{
			TypeRenderShaders.emplace(EShaderRenderType::LIT, Shader);
		}
	}

	CompileShaderSource ShaderCompiler::Build(const ShaderAsset& Asset, const ShaderVariantKey& ShaderVarient)
	{
		CompileShaderSource Result;

		SourceShader LitShader = TypeRenderShaders[ShaderVarient.ModeRender];

		uint64 Pos = LitShader.VertexShader.find(VertexShaderKeyWord);
		if (Pos == String::npos)
		{
			EG_LOG(ShaderCompilerLog, ELevelLog::WARNING, "Can't find {}", VertexShaderKeyWord);
			return Result;
		}

		Result.VertexShader =
			LitShader.VertexShader.substr(0, Pos) + Asset.CustomShader.VertexShader + LitShader.VertexShader.substr(Pos + VertexShaderKeyWord.size());

		Pos = LitShader.FragmentShader.find(FragmentShaderKeyWord);
		if (Pos == String::npos)
		{
			EG_LOG(ShaderCompilerLog, ELevelLog::WARNING, "Can't find {}", FragmentShaderKeyWord);
			return Result;
		}

		Result.FragmentShader =
			LitShader.FragmentShader.substr(0, Pos) + Asset.CustomShader.FragmentShader + LitShader.FragmentShader.substr(Pos + FragmentShaderKeyWord.size());

		return Result;
	}

	ShaderCompiler& ShaderCompiler::Get()
	{
		static ShaderCompiler Instance;
		return Instance;
	}

} // namespace CoreEngine::Render
