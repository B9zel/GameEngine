#include <Render/includes/ShaderAsset.h>
#include <Core/includes/Memory/SerializeArchive.h>

namespace CoreEngine::Render
{
	/*void ShaderAsset::Serialize(SerializeAchive& Achive)
	{
		Achive.SerializeData("Type", GetAssetType());
		Achive.PushPrefix("Shaders");
		Achive.SerializeData("Vertex", CustomShader.VertexShader);
		Achive.SerializeData("Fragment", CustomShader.FragmentShader);
	}

	void ShaderAsset::Deserialize(SerializeAchive& Achive)
	{
		Achive.PushPrefix("Shaders");
		bool IsSuccess = false;
		CustomShader.VertexShader = Achive.DeserializeData<String>("Vertex", IsSuccess);
		CustomShader.FragmentShader = Achive.DeserializeData<String>("Fragment", IsSuccess);
	}*/

} // namespace CoreEngine::Render

uint64 ShaderAsset::GetHash() const
{
	std::hash<String> Hasher;
	static String CacheString;
	CacheString = CustomShader.VertexShader;
	CacheString += CustomShader.FragmentShader;

	return Hasher(CacheString);
}
void ShaderAsset::OnSerialize(CoreEngine::SerializeAchive& Achive)
{
	Object::OnSerialize(Achive);

	Achive.PushPrefix("Shaders");
	Achive.SerializeData("Vertex", CustomShader.VertexShader);
	Achive.SerializeData("Fragment", CustomShader.FragmentShader);
}

void ShaderAsset::OnDeserialize(CoreEngine::SerializeAchive& Achive)
{
	Object::OnDeserialize(Achive);

	Achive.PushPrefix("Shaders");
	bool IsSuccess = false;
	CustomShader.VertexShader = Achive.DeserializeData<String>("Vertex", IsSuccess);
	CustomShader.FragmentShader = Achive.DeserializeData<String>("Fragment", IsSuccess);
}

CoreEngine::EAssetType ShaderAsset::GetAssetType() const
{
	return CoreEngine::EAssetType::Shader;
}
