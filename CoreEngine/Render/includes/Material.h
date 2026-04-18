#pragma once
#include <Render/includes/MaterialInterface.h>
#include <Render/includes/UniformType.h>
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>
#include <Render/includes/ShaderAsset.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Render/includes/Render.h>
#include <Material.generated.h>

namespace CoreEngine::Render
{
	class Shader;
	class Render;

} // namespace CoreEngine::Render
RCLASS()
class RMaterial : public MaterialInterface
{
	GENERATED_BODY()

public:

	RMaterial(const CoreEngine::InitializeObject& Initilize);

	const CoreEngine::Render::RHI::ShaderHandle& GetShaderHandle() const;
	const ShaderAsset* GetShaderAsset() const;
	EShaderRenderType GetModeRender() const;

	void SetShaderAsset(ShaderAsset* NewShaderAsset);
	void SetModeRender(const EShaderRenderType NewMode);
	void SetShaderHandle(const CoreEngine::Render::RHI::ShaderHandle& NewHandle);

protected:

	UniquePtr<CoreEngine::Render::BaseMaterialProperty> CreatePropertyFromType(const EUniformType& Type);

private:

	RPROPERTY();
	ShaderAsset* m_ShaderAsset;
	CoreEngine::Render::RHI::ShaderHandle m_HandleShader;
	EShaderRenderType m_ModeRender = EShaderRenderType::LIT;
};
