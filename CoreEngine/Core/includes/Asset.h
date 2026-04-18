#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Asset.generated.h>

namespace CoreEngine
{
	class SerializeAchive;

	enum class EAssetType : uint16
	{
		None = 0,
		Texture,
		Model,
		Material,
		Shader,
	};

} // namespace CoreEngine

RCLASS()
class Asset : public Object
{
	GENERATED_BODY()

public:

	Asset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual CoreEngine::EAssetType GetAssetType() const = 0;
	const String& GetPathToAsset() const;

protected:

	RPROPERTY();
	String PathToAsset;
};
