#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/Render.h>
#include <glad/glad.h>
#include <Math/includes/Matrix.h>
#include <Platform/Renderer/OpenGL/include/OpenGLFramebuffer.h>
#include <Runtime/includes/Enums/TypeLight.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShaderStorageBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLFramebufferArray.h>
#include <Render/includes/RenderCommand.h>

namespace CoreEngine
{
	struct SimplyDirectionLightProxy;
	struct SimplyPointLightProxy;
	struct SimpleSpotLightProxy;

	namespace Render
	{
		class Shader;
		struct RenderCommand;

		namespace OpenGL
		{
			struct LightShadowData
			{
				bool operator==(int ID) const
				{
					return IDLight == ID;
				}

				bool operator!=(int ID) const
				{
					return !this->operator==(ID);
				}

			public:

				int32 Layer;
				int IDLight;
			};

			class OpenGLRender : public Render
			{
			public:

				OpenGLRender();

			public:

				virtual void Construct() override;
				virtual void ClearBuffersScreen() override;
				virtual void SetViewProjectionMatrix(const FMatrix4x4& View, const FMatrix4x4& Projection) override;
				virtual void RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives, const DArray<LightProxy*>& Lights) override;
				virtual Framebuffer* GetRenderSceneBuffer() const;
				virtual void SetResolutionScale(const FVector2 Resolition) override;

			protected:

				virtual void RenderStaticMeshProxy(const StaticMeshProxy* Proxy, const DArray<LightProxy*>& Lights,
												   const DArray<FMatrix4x4>& LightDirecion) override;
				virtual void RenderProxy(const PrimitiveProxy* Proxy) override;

			private:

				void DrawDepthShadowBuffer(const DArray<LightProxy*>& Lights, const DArray<CoreEngine::PrimitiveProxy*>& Primitives);
				FMatrix4x4 DrawDirectionLightShadowBuffer(CoreEngine::LightProxy* Light, const DArray<CoreEngine::PrimitiveProxy*>& Primitives);
				FMatrix4x4 DrawSpotLightShadowBuffer(CoreEngine::LightProxy* Light, const DArray<CoreEngine::PrimitiveProxy*>& Primitives);

				SharedPtr<Framebuffer> CreateShadowBuffer();
				LightShadowData* FindLightShadowData(const ETypeLight TypeLight, const int ID) const;

				void BuidCommandList(RenderDevice* Device, const DArray<PrimitiveProxy*> Primitives, const DArray<SimplyDirectionLightProxy>& DirectionLights,
									 const DArray<SimplyPointLightProxy>& PointLights, const DArray<SimpleSpotLightProxy>& SpotLights,
									 DArray<UniquePtr<RenderCommand>>& OutCommands);

				void BuildStaticMeshCommandList(RenderDevice* Device, const StaticMeshProxy* Primitive,
												const DArray<SimplyDirectionLightProxy>& DirectionLights, const DArray<SimplyPointLightProxy>& PointLights,
												const DArray<SimpleSpotLightProxy>& SpotLights, DArray<UniquePtr<RenderCommand>>& OutCommands);

				void ExecuteCommandList(RenderDevice* Device, const DArray<UniquePtr<RenderCommand>>& Commands);

				void CollectDataFromProxy(const DArray<LightProxy*>& Lights, DArray<SimplyDirectionLightProxy>& OutDirectionLights,
										  DArray<SimplyPointLightProxy>& OutPointLights, DArray<SimpleSpotLightProxy>& OutSpotLights);

			private:

				FMatrix4x4 m_View;
				FMatrix4x4 m_Projection;
				OpenGLShaderStorageBufferObject m_SSBODirectionLight;
				OpenGLShaderStorageBufferObject m_SSBOLightSpace;
				OpenGLShaderStorageBufferObject m_SSBOPointLight;
				OpenGLShaderStorageBufferObject m_SSBOSpotLight;
				HashTableMap<ETypeLight, DArray<LightShadowData>> m_LightsFramebuffer;
				SharedPtr<OpenGL::OpenGLFramebufferArray> ShadowDepth;

				DArray<FMatrix4x4> LightsMatrix;

				SharedPtr<Framebuffer> m_ShadowBuffer;
				SharedPtr<Framebuffer> m_ResultScene;
				Shader* m_ShaderShadow;
				FVector2 CurrentRes;

				/////////////////////////////////////
				DArray<UniquePtr<RenderCommand>> m_ListCommand;
			};
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
