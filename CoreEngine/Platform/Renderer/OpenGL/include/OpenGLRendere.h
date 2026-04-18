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

class RMaterial;

namespace CoreEngine
{
	struct SimplyDirectionLightProxy;
	struct SimplyPointLightProxy;
	struct SimpleSpotLightProxy;

	namespace Render
	{
		class Shader;
		class MaterialManager;
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
				int32 IDLight;
			};

			struct RenderCommandPool
			{
			public:

				struct CeilPool
				{
					CeilPool() = default;

				public:

					bool IsUsed = false;
					UniquePtr<RenderCommand> Command;
					TypeIndex Type = typeid(nullptr);
				};

			public:

				template <class T, class... Args> T* RequestCommand(Args&&... args)
				{
					if (!m_CommandPool.count(typeid(T)))
					{
						m_CommandPool.emplace(typeid(T), DArray<CeilPool>());
					}
					DArray<CeilPool>& ArrCeil = m_CommandPool[typeid(T)];
					for (auto& Ceil : ArrCeil)
					{
						if (!Ceil.IsUsed && Ceil.Type == typeid(T))
						{
							Ceil.IsUsed = true;
							Allocator::Destruct(Ceil.Command.get());
							auto* CorrentCommand = static_cast<T*>(Ceil.Command.get());
							Allocator::Construct(CorrentCommand, std::forward<Args>(args)...);
							return CorrentCommand;
						}
					}
					CeilPool NewCeil;
					NewCeil.IsUsed = true;
					NewCeil.Type = typeid(T);
					NewCeil.Command = MakeUniquePtr<T>(std::forward<Args>(args)...);
					ArrCeil.push_back(std::move(NewCeil));
					return static_cast<T*>(ArrCeil.back().Command.get());
				}

				void ReturnCommand(RenderCommand* Command);
				void ClearCommand();

			private:

				HashTableMap<TypeIndex, DArray<CeilPool>> m_CommandPool;
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
									 DArray<RenderCommand*>& OutCommands);

				void BuildStaticMeshCommandList(RenderDevice* Device, const StaticMeshProxy* Primitive,
												const DArray<SimplyDirectionLightProxy>& DirectionLights, const DArray<SimplyPointLightProxy>& PointLights,
												const DArray<SimpleSpotLightProxy>& SpotLights, DArray<RenderCommand*>& OutCommands);

				void ExecuteCommandList(RenderDevice* Device, const DArray<RenderCommand*>& Commands);

				void CollectDataFromProxy(const DArray<LightProxy*>& Lights, DArray<SimplyDirectionLightProxy>& OutDirectionLights,
										  DArray<SimplyPointLightProxy>& OutPointLights, DArray<SimpleSpotLightProxy>& OutSpotLights);

				// Shadow
				void BuildShadowDepthCommandList(RenderDevice* Device, const DArray<LightProxy*>& Lights, const DArray<CoreEngine::PrimitiveProxy*>& Primitives,
												 DArray<RenderCommand*>& OutCommands);
				void BuildDirectionLightShadowDepthCommandList(RenderDevice* Device, LightProxy* Lights, const DArray<CoreEngine::PrimitiveProxy*>& Primitives,
															   DArray<FMatrix4x4>& LightSpace, DArray<RenderCommand*>& OutCommands);
				void BuildSpotLightShadowDepthCommandList(RenderDevice* Device, LightProxy* Lights, const DArray<CoreEngine::PrimitiveProxy*>& Primitives,
														  DArray<FMatrix4x4>& LightSpace, DArray<RenderCommand*>& OutCommands);
				void ExecuteShadowDepthCommand(RenderDevice* Device, const DArray<LightProxy*>& Lights, const DArray<RenderCommand*>& Commands);

			

			private:

				FMatrix4x4 m_View;
				FMatrix4x4 m_Projection;
				OpenGLShaderStorageBufferObject m_SSBODirectionLight;
				OpenGLShaderStorageBufferObject m_SSBOLightSpace;
				OpenGLShaderStorageBufferObject m_SSBOPointLight;
				OpenGLShaderStorageBufferObject m_SSBOSpotLight;
				HashTableMap<ETypeLight, DArray<LightShadowData>> m_LightsFramebuffer;
				Shader* m_ShaderShadow;

				DArray<FMatrix4x4> LightsMatrix;

				SharedPtr<Framebuffer> m_ShadowBuffer;
				SharedPtr<Framebuffer> m_ResultScene;
				FVector2 CurrentRes;

				/////////////////////////////////////
				DArray<RenderCommand*> m_ListCommand;
				DArray<RenderCommand*> m_ShadowDepthListCommand;

				uint32 m_CurrentShadowFramebufferLayer = 0;
				RenderCommandPool CommandPool;

			public:

				SharedPtr<OpenGL::OpenGLFramebufferArray> ShadowDepth;
			};
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
