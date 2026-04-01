#include <Core/includes/AssetManager.h>
#include <Render/includes/Texture.h>
#include <Core/includes/Engine.h>
#include <Render/includes/Render.h>
#include <Render/includes/RenderDevice.h>
#include <Render/includes/Shader.h>
#include <Core/includes/FileManager.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>

namespace CoreEngine
{
	DECLARE_LOG_CATEGORY_EXTERN(AssetManagerLog);

	AssetManager::AssetManager(const InitializeObject& Initilize) : Runtime::Object(Initilize)
	{
	}

	AssetManager::~AssetManager()
	{
		ClearAllAssets();
	}

	Render::Texture2D* AssetManager::LoadTexture(const String& Path)
	{
		UniquePtr<Render::Texture2D> NewTex = Render::Texture2D::Create(Engine::Get()->GetRender()->GetRenderDevice().get(), Path);
		auto It = Textures.insert(std::move(NewTex));

		return It.first->get();
	}

	Render::Shader* AssetManager::LoadShader(const String& VertexSh, const String& FragShad)
	{
		const String Key = VertexSh + FragShad;
		if (Shaders.count(Key))
		{
			return Shaders[Key].get();
		}

		UniquePtr<Render::Shader> NewShad = Render::Shader::CreateShader();
		NewShad->CompileShader(Engine::Get()->GetRender()->GetRenderDevice().get(), VertexSh, FragShad);

		auto It = Shaders.insert(Pair<String, UniquePtr<Render::Shader>>(Key, std::move(NewShad)));

		return It.first->second.get();
	}

	Render::Shader* AssetManager::LoadShader(const String& Path)
	{
		SourceShader LoadedShaders = LoadShaderFromFile(Path);

		return LoadShader(LoadedShaders.VertexShader, LoadedShaders.FragmentShader);
	}

	void AssetManager::ClearAllAssets()
	{
		const UniquePtr<Render::RenderDevice>& Device = Engine::Get()->GetRender()->GetRenderDevice();
		for (auto& Texture : Textures)
		{
			Device->DeleteTexture2D(Texture->GetTextureHandle());
		}
	}

	SourceShader AssetManager::LoadShaderFromFile(const String& Path)
	{
		const String& shader = FileManager::ReadFile(Path.c_str());
		SourceShader Result;

		size_t vertexPos = shader.find(Render::Shader::VertexShaderPoint);
		if (vertexPos == String::npos)
		{
			EG_LOG(AssetManagerLog, ELevelLog::ERROR, "The string \"{0}\" was't found", Render::Shader::VertexShaderPoint);
			return SourceShader();
		}

		size_t fragmentPos = shader.find(Render::Shader::FragmentShaderPoint);
		if (fragmentPos == String::npos)
		{
			EG_LOG(AssetManagerLog, ELevelLog::ERROR, "The string \"{0}\" was't found", Render::Shader::FragmentShaderPoint);
			return SourceShader();
		}

		Result.VertexShader =
			std::move(shader.substr(vertexPos + Render::Shader::VertexShaderPoint.size(), fragmentPos - Render::Shader::FragmentShaderPoint.size()));
		Result.FragmentShader = std::move(shader.substr(fragmentPos + Render::Shader::FragmentShaderPoint.size()));

		return Result;
	}

	AssetManager& AssetManager::Get()
	{
		AssetManager* SingletonManager = Engine::Get()->GetAssetManager();
		if (SingletonManager)
		{
			return *SingletonManager;
		}

		EG_LOG(AssetManagerLog, ELevelLog::CRITICAL, "Asset manager don't exist");
		return *Runtime::CreateObject<AssetManager>();
	}

} // namespace CoreEngine
