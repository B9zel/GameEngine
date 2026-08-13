#include <Core/includes/AssetManager.h>
#include <Render/includes/Texture.h>
#include <Core/includes/Engine.h>
#include <Render/includes/Render.h>
#include <Render/includes/RenderDevice.h>
#include <Render/includes/Shader.h>
#include <Core/includes/FileManager.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Render/includes/MaterialAsset.h>
#include <Core/includes/MemoryManager.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/World.h>

DECLARE_LOG_CATEGORY_EXTERN(AssetManagerLog);

AssetManager::AssetManager(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
{
}

AssetManager::~AssetManager()
{
	ClearAllAssets();
}

CoreEngine::Render::Texture2D* AssetManager::LoadTexture(const String& Path)
{

	UniquePtr<CoreEngine::Render::Texture2D> NewTex = CoreEngine::Render::Texture2D::Create(Engine::Get()->GetRender()->GetRenderDevice().get(), Path);
	auto It = Textures.insert(std::move(NewTex));

	return It.first->get();
}

CoreEngine::Render::Shader* AssetManager::LoadShader(const String& VertexSh, const String& FragShad)
{
	const String Key = VertexSh + FragShad;
	if (Shaders.count(Key))
	{
		return Shaders[Key].get();
	}

	UniquePtr<CoreEngine::Render::Shader> NewShad = CoreEngine::Render::Shader::CreateShader();
	NewShad->CompileShader(Engine::Get()->GetRender()->GetRenderDevice().get(), VertexSh, FragShad);
	if (!NewShad->GetIsCompile()) return nullptr;

	auto It = Shaders.insert(Pair<String, UniquePtr<CoreEngine::Render::Shader>>(Key, std::move(NewShad)));

	return It.first->second.get();
}

CoreEngine::Render::Shader* AssetManager::LoadShader(const String& Path)
{
	SourceShader LoadedShaders = LoadStringShaderFromFile(Path);
	auto* Shader = LoadShader(LoadedShaders.VertexShader, LoadedShaders.FragmentShader);

	if (!Shader)
	{
		EG_LOG(AssetManagerLog, ELevelLog::ERROR, "Can't load shader: {}", Path);
	}

	return Shader;
}

void AssetManager::ClearAllAssets()
{
	const UniquePtr<CoreEngine::Render::RenderDevice>& Device = Engine::Get()->GetRender()->GetRenderDevice();
	for (auto& Texture : Textures)
	{
		Device->DeleteTexture2D(Texture->GetTextureHandle());
	}
}

void AssetManager::PreChangeNameOfAsset(Asset* asset, CoreEngine::Reflection::PropertyField& Field)
{
	if (Field.Name == "Name")
	{
		/*for (size_t i = 0; i < m_Assets.size(); i++)
		{
			for (auto* field : m_Assets[i]->GetClass()->GetWithParentPropertyFields())
			{
				if (Field == (*field))
				{*/
		if (auto It = LoadedAssets.find(asset->GetPathToAsset()); It != LoadedAssets.end())
		{
			AssetChangingName = *It;
		}
		//}
		//}
		//}
	}
}

void AssetManager::PostChangeNameOfAsset(Asset* asset, CoreEngine::Reflection::PropertyField& Field)
{
	if (Field.Name == "Name")
	{
		auto Node = LoadedAssets.extract(AssetChangingName.first);
		Node.key() = m_Assets[AssetChangingName.second]->GetPathToAsset();
		LoadedAssets.insert(std::move(Node));
	}
}

SourceShader AssetManager::LoadStringShaderFromFile(const String& Path)
{
	const String& shader = CoreEngine::FileManager::ReadFile(Path.c_str());
	SourceShader Result;

	size_t vertexPos = shader.find(CoreEngine::Render::Shader::VertexShaderPoint);
	if (vertexPos == String::npos)
	{
		EG_LOG(AssetManagerLog, ELevelLog::ERROR, "The string \"{0}\" was't found", CoreEngine::Render::Shader::VertexShaderPoint);
		return SourceShader();
	}

	size_t fragmentPos = shader.find(CoreEngine::Render::Shader::FragmentShaderPoint);
	if (fragmentPos == String::npos)
	{
		EG_LOG(AssetManagerLog, ELevelLog::ERROR, "The string \"{0}\" was't found", CoreEngine::Render::Shader::FragmentShaderPoint);
		return SourceShader();
	}

	Result.VertexShader = std::move(
		shader.substr(vertexPos + CoreEngine::Render::Shader::VertexShaderPoint.size(), fragmentPos - CoreEngine::Render::Shader::FragmentShaderPoint.size()));
	Result.FragmentShader = std::move(shader.substr(fragmentPos + CoreEngine::Render::Shader::FragmentShaderPoint.size()));

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
	return *CreateObject<AssetManager>();
}

Asset* AssetManager::LoadAsset(const String& Path)
{
	auto& ExistAsset = LoadedAssets.find(Path);
	if (ExistAsset != LoadedAssets.end())
	{
		return m_Assets[ExistAsset->second];
	}

	CoreEngine::SerializeAchive Achive;
	CoreEngine::EAssetType TypeAsset = Engine::Get()->GetWorld()->GetSaveManager()->LoadFileAsset(Path, Achive);
	if (TypeAsset == CoreEngine::EAssetType::None) return nullptr;

	Asset* NewAsset = CoreEngine::AssetFactory::CreateAsset(TypeAsset);
	CoreEngine::MemoryManager::GetInstance()->GetGarbageCollector()->AddRootObject(NewAsset);
	NewAsset->PreDeserialize();
	NewAsset->Deserialize(Achive);

	NewAsset->PreChangeProperty.AddBind(&AssetManager::PreChangeNameOfAsset, this);
	NewAsset->PostChangeProperty.AddBind(&AssetManager::PostChangeNameOfAsset, this);

	m_Assets.push_back(NewAsset);
	LoadedAssets.emplace(Path, m_Assets.size() - 1);

	return NewAsset;
}

Asset* AssetManager::CreateAsset(const String& Path, CoreEngine::EAssetType Type)
{
	auto& ExistAsset = LoadedAssets.find(Path);
	if (ExistAsset != LoadedAssets.end())
	{
		return nullptr;
	}

	Asset* NewAsset = CoreEngine::AssetFactory::CreateAsset(Type);
	CoreEngine::MemoryManager::GetInstance()->GetGarbageCollector()->AddRootObject(NewAsset);

	NewAsset->SetPathToAsset(Path);
	m_Assets.push_back(NewAsset);
	LoadedAssets.emplace(Path, m_Assets.size() - 1);

	return NewAsset;
}

const DArray<Asset*> AssetManager::GetLoadedAssets() const
{
	return m_Assets;
}
