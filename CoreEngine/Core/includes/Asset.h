#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Asset.generated.h>

class Asset;

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

	class AssetFactory
	{
	public:

		static Asset* CreateAsset(const EAssetType& Type);
	};
} // namespace CoreEngine

RCLASS()
class Asset : public Object
{
	GENERATED_BODY()

public:

	Asset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual CoreEngine::EAssetType GetAssetType() const; // Override this function in derived classes to return the specific asset type
	const String& GetPathToAsset() const;
	void SetPathToAsset(const String& NewPath);

	virtual void PreEditChangeProperty(CoreEngine::Reflection::PropertyField& Property) override;
	virtual void PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property) override;
	virtual void SetName(const String& NewName) override;
	virtual void OnSerialize(CoreEngine::SerializeAchive& Archive) override;

public:

	Dispatcher<Asset*, CoreEngine::Reflection::PropertyField&> PreChangeProperty;
	Dispatcher<Asset*,CoreEngine::Reflection::PropertyField&> PostChangeProperty;

protected:

	RPROPERTY();
	String PathToAsset;
};
