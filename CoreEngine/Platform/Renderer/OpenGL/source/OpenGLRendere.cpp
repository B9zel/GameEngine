#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>
#include <Core/includes/Base.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/StaticMeshProxy.h>
#include <Core/includes/LightProxy.h>
#include <Core/includes/Engine.h>
#include <Core/includes/World.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRenderCommand.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Core/includes/Window.h>
#include <Render/includes/Framebuffer.h>
#include <Render/includes/Shader.h>
#include <Core/includes/AssetManager.h>
#include <Render/includes/Material.h>
#include <Render/includes/ShaderCache.h>
#include <Render/includes/ShaderCompiler.h>
#include <Render/includes/ShaderAsset.h>
#include <Core/includes/Application.h>
#include <Render/includes/MaterialManager.h>
#include "glad/glad.h"

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLRender::OpenGLRender()
			{
				m_SSBODirectionLight.CreaterBuffer(nullptr, 0, sizeof(SimplyDirectionLightProxy), ETypeStorageDraw::STREAM);
				m_SSBOPointLight.CreaterBuffer(nullptr, 0, sizeof(SimplyPointLightProxy), ETypeStorageDraw::STREAM);
				m_SSBOSpotLight.CreaterBuffer(nullptr, 0, sizeof(SimpleSpotLightProxy), ETypeStorageDraw::STREAM);
				m_SSBOLightSpace.CreaterBuffer(nullptr, 0, sizeof(FMatrix4x4), ETypeStorageDraw::STREAM);

				CurrentRes.x = Application::Get()->GetWindow().GetWidth();
				CurrentRes.y = Application::Get()->GetWindow().GetHeight();

				FramebufferArraySpecification SpecShadow;
				SpecShadow.Height = Application::Get()->GetWindow().GetHeight();
				SpecShadow.Width = Application::Get()->GetWindow().GetWidth();

				FramebufferTextureAttachment Tex(EFramebufferTextureFormat::DEPTH_COMPONENT);
				Tex.Filter = EFramebufferTextureFilterConfig::NEAREST;
				Tex.Wrap = EFramebufferTextureWrapConfig::CLAMP_TO_BORDER;
				Tex.Color = FVector4(1.0f);
				SpecShadow.AttachTextures.Textures.push_back(Tex);
				SpecShadow.LayerCount = 64;
				ShadowDepth = MakeSharedPtr<OpenGL::OpenGLFramebufferArray>(SpecShadow);

				CoreEngine::Render::FramebufferSpecification Spec;
				Spec.Height = Application::Get()->GetWindow().GetHeight();
				Spec.Width = Application::Get()->GetWindow().GetWidth();
				Spec.AttachTextures.Textures.push_back(CoreEngine::Render::FramebufferTextureAttachment(EFramebufferTextureFormat::RGBA8));
				Spec.AttachTextures.Textures.push_back(CoreEngine::Render::FramebufferTextureAttachment(EFramebufferTextureFormat::RED_INTEGER));
				Spec.AttachTextures.Textures.push_back(
					CoreEngine::Render::FramebufferTextureAttachment(EFramebufferTextureFormat::DEPTH24_STENCIL8, EFramebufferTextureFilterConfig::NEAREST));

				m_ResultScene = Framebuffer::Create(Spec);
			}

			void OpenGLRender::RenderStaticMeshProxy(const StaticMeshProxy* Proxy, const DArray<LightProxy*>& Lights, const DArray<FMatrix4x4>& LightDirecion)
			{
				// DArray<SimplyDirectionLightProxy> DirectionLights;
				// DArray<SimplyPointLightProxy> PointLights;
				// DArray<SimpleSpotLightProxy> SpotLights;
				// DArray<FMatrix4x4> LightSpaces;

				// for (int64 i = 0; i < Lights.size(); i++)
				//{
				//	DArray<GLuint64> Handles;
				//	auto* ShadowData = FindLightShadowData(Lights[i]->GetTypeLight(), Lights[i]->GetID());

				//	switch (Lights[i]->GetTypeLight())
				//	{
				//	case ETypeLight::DIRECTION_LIGHT:
				//	{
				//		DirectionLightProxy* Proxy = dynamic_cast<DirectionLightProxy*>(Lights[i]);
				//		SimplyDirectionLightProxy SimplePointLight = {Proxy->GetColor(), Proxy->GetIntencity(), Proxy->GetDirection(), ShadowData->Layer};
				//		DirectionLights.emplace_back(SimplePointLight);
				//		break;
				//	}
				//	case ETypeLight::POINT_LIGHT:
				//	{
				//		PointLightProxy* Proxy = dynamic_cast<PointLightProxy*>(Lights[i]);
				//		SimplyPointLightProxy SimplePointLight = {Proxy->GetColor(),	Proxy->GetIntencity(), Proxy->GetLocation(),
				//												  Proxy->GetConstant(), Proxy->GetLinear(),	   Proxy->GetQuadratic()};
				//		PointLights.emplace_back(SimplePointLight);
				//		break;
				//	}
				//	case ETypeLight::SPOTLIGHT:
				//	{
				//		SpotLightProxy* Proxy = dynamic_cast<SpotLightProxy*>(Lights[i]);
				//		SimpleSpotLightProxy SimpleSpotLight = {
				//			Proxy->GetColor(),	   Proxy->GetDirection(), Proxy->GetLocation(),	   ShadowData ? ShadowData->Layer : 0,
				//			Proxy->GetIntencity(), Proxy->GetCutOff(),	  Proxy->GetOuterCutOff(), Proxy->GetConstant(),
				//			Proxy->GetLinear(),	   Proxy->GetQuadratic()};
				//		SpotLights.emplace_back(SimpleSpotLight);
				//		break;
				//	}
				//	default:
				//		break;
				//	}
				//	// LightSpaces.push_back(LightsMatrix[DirectionLights.size()]);
				//}

				// m_SSBODirectionLight.CreaterBuffer(DirectionLights.data(), DirectionLights.size(), sizeof(SimplyDirectionLightProxy),
				// ETypeStorageDraw::STREAM); m_SSBOPointLight.CreaterBuffer(PointLights.data(), PointLights.size(), sizeof(SimplyPointLightProxy),
				// ETypeStorageDraw::STREAM); m_SSBOSpotLight.CreaterBuffer(SpotLights.data(), SpotLights.size(), sizeof(SimpleSpotLightProxy),
				// ETypeStorageDraw::STREAM); m_SSBOLightSpace.CreaterBuffer(LightsMatrix.data(), LightsMatrix.size(), sizeof(FMatrix4x4),
				// ETypeStorageDraw::STREAM);
				//  EG_LOG(CoreEngine::CORE, ELevelLog::INFO, alignof(PointLightProxy));
				//  glDisable(GL_BLEND);

				// int test = 0;
				// for (size_t i = 0; i < Proxy->GetIndeces().size(); i++)
				//{
				//	for (auto& el : Proxy->GetShaders())
				//	{

				//		// if (test++ == 0) continue;
				//		auto* shader = el.first;
				//		auto* vertexArray = Proxy->GetArrayObject()[i];

				//		vertexArray->Bind();
				//		shader->Bind();

				//		auto& Textures = Proxy->GetTextures();

				//		for (uint32 j = 0; j < Textures.size(); j++)
				//		{
				//			Textures[j]->Bind(j);

				//			const DArray<String>& NamesTexture = shader->GetNamesOfTexture();
				//			if (!NamesTexture.empty())
				//			{
				//				if (j >= NamesTexture.size()) continue;

				//				shader->SetUniform1i(NamesTexture[j], j, false);
				//			}
				//		}

				//		if (shader->GetHasAllMatrix())
				//		{
				//			FVector4 a = m_Projection * m_View * Proxy->GetTransformMatrix() * FVector4(1, 1, 1, 1).vector;
				//			// FMatrix4x4 b = m_Projection * m_View * Proxy->GetTransform().ToMatrix();

				//			shader->SetUniformMatrix4x4("Model", Proxy->GetTransformMatrix(), false);

				//			shader->SetUniformMatrix4x4("View", m_View, false);
				//			shader->SetUniformMatrix4x4("Projection", m_Projection, false);
				//		}

				//		glActiveTexture(GL_TEXTURE0);
				// ShadowDepth->ActivateDepthTexture();
				//		shader->SetUniform1i("DirectionShadowMap", 0, false);
				//		shader->SetUniformVec3("ViewPos", Engine::Get()->GetWorld()->GetControllerLocation(), false);
				//		shader->SetUniform1i("CountPointLight", PointLights.size(), false);
				//		shader->SetUniform1i("CountDirectionLight", DirectionLights.size(), false);
				//		shader->SetUniform1i("CountSpotLight", SpotLights.size(), false);
				//		// shader->SetUniform1i("CountDirectionLight", ¬шку, false);

				//		// shader->SetUniformVec2("ScreenSize", FVector2(Application::Get()->GetWindow().GetWidth(),
				//		// Application::Get()->GetWindow().GetHeight())); shader->SetUniform1i("ShadowMap", 1);

				//		/*if (Proxy->GetUUID()->IsValid())
				//		{
				//			shader->SetUniform1i("id", Proxy->GetUUID()->GetID(), false);

				//		}
				//		else
				//		{

				//			shader->SetUniform1i("id", 0, false);
				//		}*/
				//		/*shader->SetUniform1i("texture", 0, false);
				//		glActiveTexture(GL_TEXTURE0);
				//		glBindTexture(GL_TEXTURE_2D, 3);*/

				//		// int b;
				//		// glGetUniformiv(3, glGetUniformLocation(3, "CountSpotLight"), &b);
				//		// EG_LOG(CORE, ELevelLog::INFO, "{0}", b);
				//		if (!Proxy->GetLocationLights().empty())
				//		{
				//			shader->SetUniformVec3("PosLight", Proxy->GetLocationLights()[0], false);
				//		}

				//		int ebo;
				//		m_SSBODirectionLight.Bind(0);
				//		m_SSBOPointLight.Bind(1);
				//		m_SSBOSpotLight.Bind(2);
				//		m_SSBOLightSpace.Bind(3);
				//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 3);
				//		// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
				//		// float* dat = nullptr;
				//		// dat = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 60, GL_MAP_READ_BIT);

				//		// if (dat)
				//		//{
				//		//	for (int i = 0; i < 60 / sizeof(float); i += 3)
				//		//	{
				//		//		EG_LOG(CORE, ELevelLog::INFO, "{0}, {1}, {2}", dat[i], dat[i + 1], dat[i + 2]);
				//		//	}
				//		// }

				//		/*unsigned int a;
				//		int locat = glGetUniformLocation(3, "ID");
				//		glGetUniformuiv(3, locat, &a);
				//		EG_LOG(CORE, ELevelLog::INFO, a);*/
				//		//	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 2);
				//		//	glGetIntegerv(GL_SHADER_STORAGE_BUFFER, &ebo);
				//		// EG_LOG(CORE, ELevelLog::INFO, ebo);
				//		// void* d = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
				//		// if (d != nullptr)
				//		//{
				//		//	PointLightProxy* l = reinterpret_cast<PointLightProxy*>(d);
				//		//	for (size_t i = 0; i < 32; i++)
				//		//	{

				//		//		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, *((double*)l + i * sizeof(double)));
				//		//	}

				//		//}
				//		glDrawElements(GL_TRIANGLES, Proxy->GetIndeces()[i]->size(), GL_UNSIGNED_INT, 0);

				//		shader->UnBind();
				//		vertexArray->UnBind();

				//		glBindTexture(GL_TEXTURE_2D, 0);
				//	}
				//	test = 0;
				//}
				// glEnable(GL_BLEND);
			}

			void OpenGLRender::Construct()
			{
				m_ShaderShadow = AssetManager::Get().LoadShader("../../Shaders/ShadowDepthShader.glsl");
			}

			void OpenGLRender::ClearBuffersScreen()
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			void OpenGLRender::SetViewProjectionMatrix(const FMatrix4x4& View, const FMatrix4x4& Projection)
			{
				m_View = View;
				m_Projection = Projection;
			}

			void OpenGLRender::RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives, const DArray<LightProxy*>& Lights)
			{
				// DrawDepthShadowBuffer(Lights, Primitives);
				DArray<SimplyDirectionLightProxy> DirectionLights;
				DArray<SimplyPointLightProxy> PointLights;
				DArray<SimpleSpotLightProxy> SpotLights;
				DArray<FMatrix4x4> LightSpaces;
				m_ListCommand.clear();
				m_ShadowDepthListCommand.clear();
				CommandPool.ClearCommand();
				LightsMatrix.clear();
				m_CurrentShadowFramebufferLayer = 0;

				BuildShadowDepthCommandList(GetRenderDevice().get(), Lights, Primitives, m_ShadowDepthListCommand);
				ExecuteShadowDepthCommand(GetRenderDevice().get(), Lights, m_ShadowDepthListCommand);

				CollectDataFromProxy(Lights, DirectionLights, PointLights, SpotLights);

				BuidCommandList(GetRenderDevice().get(), Primitives, DirectionLights, PointLights, SpotLights, m_ListCommand);
				ExecuteCommandList(GetRenderDevice().get(), m_ListCommand);

				/*m_ResultScene->Bind();
				ClearBuffersScreen();
				for (auto* Primitive : Primitives)
				{
					if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Primitive))
					{
						RenderStaticMeshProxy(StaticProxy, Lights, LightsMatrix);
					}
					else
					{
						RenderProxy(Primitive);
					}
				}
				m_ResultScene->UnBind();*/
			}

			void OpenGLRender::DrawDepthShadowBuffer(const DArray<LightProxy*>& Lights, const DArray<CoreEngine::PrimitiveProxy*>& Primitives)
			{
				// Shadow depth
				uint32 SumSize = 0;
				for (auto& It : m_LightsFramebuffer)
				{
					SumSize += It.second.size();
				}
				if (Lights.size() != SumSize)
				{
					m_LightsFramebuffer.clear();
				}
				LightsMatrix.clear();

				uint32 LayerIndex = 0;
				// m_ShaderShadow->Bind();
				ShadowDepth->Bind();
				glCullFace(GL_FRONT);
				for (auto* Light : Lights)
				{
					if (Light->GetTypeLight() == ETypeLight::POINT_LIGHT) continue;
					auto* CreateBuffer = FindLightShadowData(Light->GetTypeLight(), Light->GetID());

					if (!CreateBuffer)
					{
						LightShadowData LightData;
						LightData.Layer = LayerIndex;
						LightData.IDLight = Light->GetID();
						CreateBuffer = &m_LightsFramebuffer[Light->GetTypeLight()].emplace_back(LightData);
					}
					ShadowDepth->BindDepthLayar(LayerIndex);
					ClearBuffersScreen();

					switch (Light->GetTypeLight())
					{
					case ETypeLight::DIRECTION_LIGHT:
						LightsMatrix.push_back(DrawDirectionLightShadowBuffer(Light, Primitives));
						break;
					case ETypeLight::SPOTLIGHT:
						LightsMatrix.push_back(DrawSpotLightShadowBuffer(Light, Primitives));
					default:
						break;
					}

					LayerIndex++;
					// CreateBuffer->ShadowDepth->UnBind();
				}

				glCullFace(GL_BACK);
				// m_ShadowBuffer->UnBind();
				// m_ShaderShadow->UnBind();
			}

			FMatrix4x4 OpenGLRender::DrawDirectionLightShadowBuffer(CoreEngine::LightProxy* Light, const DArray<CoreEngine::PrimitiveProxy*>& Primitives)
			{
				auto* Direction = dynamic_cast<DirectionLightProxy*>(Light);
				FMatrix4x4 LightSpace(1);
				if (!Direction) return LightSpace;

				FMatrix4x4 ProjectionLight = Math::CreateOrthoMatrix(-300, 300, -300, 300, 0.0f, 1000.5f);
				FVector DirectionLight = -(Direction->GetDirection() * 100);

				FVector PosView;
				Math::DecomposeLocationMatrix(m_View, PosView);

				FMatrix4x4 ViewLight = Math::CreateMatrixLookAt(DirectionLight, Direction->GetDirection(), FVector::UpVector);
				LightSpace = ProjectionLight * ViewLight;

				for (auto* Proxy : Primitives)
				{
					if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Proxy))
					{
						for (size_t i = 0; i < StaticProxy->GetIndeces().size(); i++)
						{
							// StaticProxy->GetArrayObject()[i]->Bind();

							using namespace CoreEngine::Render;

							// m_ShaderShadow->SetUniformMatrix4x4("LightSpaceMat", LightSpace, false);
							// m_ShaderShadow->SetUniformMatrix4x4("ModelMat", StaticProxy->GetTransformMatrix(), false);

							glDrawElements(GL_TRIANGLES, StaticProxy->GetIndeces()[i]->size(), GL_UNSIGNED_INT, 0);
						}
					}
				}
				return LightSpace;
			}

			void OpenGLRender::BuildDirectionLightShadowDepthCommandList(RenderDevice* Device, LightProxy* Lights,
																		 const DArray<CoreEngine::PrimitiveProxy*>& Primitives, DArray<FMatrix4x4>& LightSpaces,
																		 DArray<RenderCommand*>& OutCommands)
			{
				auto* DirectionLight = static_cast<DirectionLightProxy*>(Lights);
				if (!DirectionLight) return;

				const FMatrix4x4 ProjectionLight = Math::CreateOrthoMatrix(-30, 30, -30, 30, 0.0f, 1000.0f);
				FVector Position;
				Math::DecomposeLocationMatrix(m_View, Position);

				FVector Pos = -(DirectionLight->GetDirection() * 100);

				const FMatrix4x4 ViewLight = Math::CreateMatrixLookAt(Pos, DirectionLight->GetDirection(), FVector::UpVector);

				// const float NearPlane = 0.1f;
				// const float FarPlane = 100.0f;

				// const FMatrix4x4 ProjectionLight = Math::CreateOrthoMatrix(-30, 30, -30, 30, NearPlane, FarPlane);
				// FVector Position;
				// Math::DecomposeLocationMatrix(m_View, Position);
				// FMatrix4x4 InvView = Math::Inverse(m_View);
				// FVector Forward = glm::vec3(InvView[2]);

				// FVector Center = Position + Forward * (NearPlane + FarPlane) * 0.5f;
				// float lightDistance = 100.0f;
				// FVector lightEyePos = Center - DirectionLight->GetDirection() * lightDistance;

				// FVector Pos = -(DirectionLight->GetDirection() * 100);

				// const FMatrix4x4 ViewLight = Math::CreateMatrixLookAt(lightEyePos, lightEyePos + DirectionLight->GetDirection(), FVector::UpVector);

				const FMatrix4x4 LightSpace = ProjectionLight * ViewLight;
				LightSpaces.push_back(LightSpace);
				OutCommands.push_back(CommandPool.RequestCommand<GLCmdBindFramebufferArray>(ShadowDepth.get(), m_CurrentShadowFramebufferLayer));
				OutCommands.push_back(CommandPool.RequestCommand<GLCmdClearColorAndDepth>());
				OutCommands.push_back(CommandPool.RequestCommand<CmdBindShaderProgram>(m_ShaderShadow->GetHandle(), []() {}));
				OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(m_ShaderShadow->GetHandle(), "LightSpaceMat", LightSpace));

				for (auto* Proxy : Primitives)
				{
					if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Proxy))
					{
						for (uint64 i = 0; i < StaticProxy->GetIndeces().size(); i++)
						{
							// OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::BIND_FRAMEBUFFER_ARRAY, ShadowDepth.get(),
							//												 m_CurrentShadowFramebufferLayer));

							// OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::CLEAR_COLOR_AND_DEPTH));

							// OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::DRAW_INDEX, StaticProxy->GetArrayObject()[i],
							//												 StaticProxy->GetIndeces()[i]->size(), []() {}));
							OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(m_ShaderShadow->GetHandle(), "ModelMat",
																									   StaticProxy->GetTransformMatrix()));

							OutCommands.push_back(
								CommandPool.RequestCommand<GLCmdDrawIndex>(StaticProxy->GetArrayObject()[i], StaticProxy->GetIndeces()[i]->size(), []() {}));
							/*OutCommands.push_back(MakeUniquePtr<GLCmdBindFramebufferArray>(ShadowDepth.get(), m_CurrentShadowFramebufferLayer));
							OutCommands.push_back(MakeUniquePtr<GLCmdClearColorAndDepth>());

							OutCommands.push_back(
								MakeUniquePtr<GLCmdDrawIndex>(StaticProxy->GetArrayObject()[i], StaticProxy->GetIndeces()[i]->size(), []() {}));*/
						}
					}
				}
			}

			FMatrix4x4 OpenGLRender::DrawSpotLightShadowBuffer(CoreEngine::LightProxy* Light, const DArray<CoreEngine::PrimitiveProxy*>& Primitives)
			{
				auto* SpotLight = dynamic_cast<SpotLightProxy*>(Light);
				FMatrix4x4 LightSpace(1);
				if (!SpotLight) return LightSpace;

				FMatrix4x4 ProjectionLight = Math::CreatePerspectiveMatrix((SpotLight->GetOuterCutOff() * 8), ShadowDepth->GetSpecifiction().Width,
																		   ShadowDepth->GetSpecifiction().Height, 0.1, 10001);
				FVector DirectionLight = (SpotLight->GetDirection());

				FMatrix4x4 ViewLight =
					Math::CreateMatrixLookAt(SpotLight->GetLocation(), SpotLight->GetLocation() + SpotLight->GetDirection() * 100, FVector::UpVector);
				LightSpace = ProjectionLight * ViewLight;

				for (auto* Proxy : Primitives)
				{
					if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Proxy))
					{
						for (size_t i = 0; i < StaticProxy->GetIndeces().size(); i++)
						{
							// StaticProxy->GetArrayObject()[i]->Bind();

							using namespace CoreEngine::Render;

							// m_ShaderShadow->SetUniformMatrix4x4("LightSpaceMat", LightSpace, false);
							// m_ShaderShadow->SetUniformMatrix4x4("ModelMat", StaticProxy->GetTransformMatrix(), false);

							glDrawElements(GL_TRIANGLES, StaticProxy->GetIndeces()[i]->size(), GL_UNSIGNED_INT, 0);
						}
					}
				}
				return LightSpace;
			}

			SharedPtr<Framebuffer> OpenGLRender::CreateShadowBuffer()
			{
				FramebufferSpecification SpecShadow;
				SpecShadow.Height = CurrentRes.y;
				SpecShadow.Width = CurrentRes.x;

				FramebufferTextureAttachment Tex(EFramebufferTextureFormat::DEPTH_COMPONENT);
				Tex.Filter = EFramebufferTextureFilterConfig::NEAREST;
				Tex.Wrap = EFramebufferTextureWrapConfig::CLAMP_TO_BORDER;
				Tex.Color = FVector4(1.0f);
				SpecShadow.AttachTextures.Textures.push_back(Tex);

				return Framebuffer::Create(SpecShadow);
			}

			LightShadowData* OpenGLRender::FindLightShadowData(const ETypeLight TypeLight, const int ID) const
			{
				if (!m_LightsFramebuffer.count(TypeLight)) return nullptr;

				DArray<LightShadowData>& ArrOfFramebuffer = const_cast<DArray<LightShadowData>&>((*m_LightsFramebuffer.find(TypeLight)).second);
				auto FindedElement =
					std::find_if(ArrOfFramebuffer.begin(), ArrOfFramebuffer.end(), [&](const LightShadowData& LightData) -> bool { return (LightData) == ID; });
				if (FindedElement == ArrOfFramebuffer.end()) return nullptr;

				return &(*FindedElement);
			}

			Framebuffer* OpenGLRender::GetRenderSceneBuffer() const
			{
				return m_ResultScene.get();
			}

			void OpenGLRender::SetResolutionScale(const FVector2 Resolition)
			{
				CurrentRes = Resolition;
				// m_ShadowBuffer->Resize(Resolition.x * 6, Resolition.y * 6);
				ShadowDepth->Resize(Resolition.x * 2, Resolition.y * 2);
				m_ResultScene->Resize(Resolition.x, Resolition.y);
			}

			void OpenGLRender::RenderProxy(const PrimitiveProxy* Proxy)
			{
				for (auto& el : Proxy->GetShaders())
				{
					// auto* shader = el.first;
					// auto* vertexArray = el.second.first;

					//// shader->Bind();
					//// vertexArray->Bind();

					// auto& Textures = Proxy->GetTextures();

					// for (uint32 i = 0; i < Textures.size(); i++)
					//{
					//	// Textures[i]->Bind(i);

					//	const DArray<String>& NamesTexture = shader->GetNamesOfTexture();
					//	if (!NamesTexture.empty())
					//	{
					//		if (i >= NamesTexture.size()) continue;

					//		// shader->SetUniform1i(NamesTexture[i], i, false);
					//	}
					//}

					// if (shader->GetHasAllMatrix())
					//{
					//	FMatrix4x4 a = m_Projection * m_View * Proxy->GetTransformMatrix();

					//	// shader->SetUniformMatrix4x4("Model", Proxy->GetTransformMatrix(), false);
					//	// shader->SetUniformMatrix4x4("View", m_View, false);
					//	// shader->SetUniformMatrix4x4("Projection", m_Projection, false);
					//}

					// glActiveTexture(GL_TEXTURE0);
					// ShadowDepth->ActivateDepthTexture();
					//// shader->SetUniform1i("ourTexture1", 0, false);

					// glDrawArrays(GL_TRIANGLES, 0, Proxy->CountVertex / 3);

					// glDrawElements(GL_TRIANGLES, Proxy->CountIndeces, GL_UNSIGNED_INT, 0);
					//// el.first->UnBind();
					//// vertexArray->UnBind();

					// glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			void OpenGLRender::BuidCommandList(RenderDevice* Device, const DArray<PrimitiveProxy*> Primitives,
											   const DArray<SimplyDirectionLightProxy>& DirectionLights, const DArray<SimplyPointLightProxy>& PointLights,
											   const DArray<SimpleSpotLightProxy>& SpotLights, DArray<RenderCommand*>& OutCommands)
			{
				// OutCommands.push_back(MakeUniquePtr<CmdBindShaderProgram>(m_ShaderShadow->GetHandle(), []() {}));
				OutCommands.push_back(CommandPool.RequestCommand<CmdBindShaderProgram>(m_ShaderShadow->GetHandle(), []() {}));
				// OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::BIND_SHADER_PROGRAM, m_ShaderShadow->GetHandle(), []() {}));

				for (auto* Primitive : Primitives)
				{
					if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Primitive))
					{
						BuildStaticMeshCommandList(Device, StaticProxy, DirectionLights, PointLights, SpotLights, OutCommands);
					}
				}
			}

			void OpenGLRender::BuildStaticMeshCommandList(RenderDevice* Device, const StaticMeshProxy* Primitive,
														  const DArray<SimplyDirectionLightProxy>& DirectionLights,
														  const DArray<SimplyPointLightProxy>& PointLights, const DArray<SimpleSpotLightProxy>& SpotLights,
														  DArray<RenderCommand*>& OutCommands)
			{

				for (size_t i = 0; i < Primitive->GetIndeces().size(); i++)
				{
					for (auto& el : Primitive->GetMaterials())
					{
						RMaterial* Mat = MaterialManager::Get().GetMaterial(el);
						if (!Mat->GetShaderAsset()) continue;

						PrepareMaterial(*Mat);
						Shader* shader = ShaderCache::GetShaderFromMaterial(*Mat);
						if (!shader) continue;

						RHI::ShaderHandle shaderHandle = shader->GetHandle();

						// OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::BIND_SHADER_PROGRAM, shaderHandle, []() {}));
						//  OutCommands.push_back(MakeUniquePtr<CmdBindShaderProgram>(shaderHandle, []() {}));
						OutCommands.push_back(CommandPool.RequestCommand<CmdBindShaderProgram>(shaderHandle, []() {}));

						// auto SetTextures = MakeUniquePtr<ActivateTextureSet>();
						const auto& Textures = Primitive->GetTextures();
						if (!Textures.empty())
						{
							// for (uint32 i = 0; i < el.TextureNames.size(); i++)
							{
								// SetTextures->Textures.emplace(i, Pair<RHI::TextureHandle, String>(Textures[i], el.TextureNames[i]));
							}
						}
						// OutCommands.push_back(MakeUniquePtr<CmdActivationTexture>(std::move(SetTextures), []() {}));

						if (shader->GetHasAllMatrix())
						{
							/*OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_MATRIX_4X4, shaderHandle, "Model",
																			 Primitive->GetTransformMatrix()));
							OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_MATRIX_4X4, shaderHandle, "View", m_View));
							OutCommands.push_back(
								CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_MATRIX_4X4, shaderHandle, "Projection", m_Projection));*/
							/*OutCommands.push_back(MakeUniquePtr<GLCmdSetUniformMatrix4x4>(shaderHandle, "Model", Primitive->GetTransformMatrix()));
							OutCommands.push_back(MakeUniquePtr<GLCmdSetUniformMatrix4x4>(shaderHandle, "View", m_View));
							OutCommands.push_back(MakeUniquePtr<GLCmdSetUniformMatrix4x4>(shaderHandle, "Projection", m_Projection));*/
							OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(shaderHandle, "Model", Primitive->GetTransformMatrix()));
							OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(shaderHandle, "View", m_View));
							OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(shaderHandle, "Projection", m_Projection));
						}

						for (auto Property : *Primitive->GetMaterialProperties())
						{
							SharedPtr<BaseMaterialProperty> Parameter = Property;

							switch (Parameter->Type)
							{
							case EUniformType::FLOAT:
							{
								auto* FloatProperty = static_cast<MaterialPropertyFloat*>(Parameter.get());
								OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniform1f>(shaderHandle, FloatProperty->Name, FloatProperty->Value));
								break;
							}
							case EUniformType::VEC3:
							{
								auto* Vector3Property = static_cast<MaterialPropertyVec3*>(Parameter.get());
								OutCommands.push_back(
									CommandPool.RequestCommand<GLCmdSetUniformVector3>(shaderHandle, Vector3Property->Name, Vector3Property->Value));
								break;
							}
							case EUniformType::INT:
							{
								auto* IntProperty = static_cast<MaterialPropertyInt*>(Parameter.get());
								OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniform1i>(shaderHandle, IntProperty->Name, IntProperty->Value));
								break;
							}
							case EUniformType::MAT4:
							{
								auto* Vector4Property = static_cast<MaterialPropertyMat4*>(Parameter.get());
								OutCommands.push_back(
									CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(shaderHandle, Vector4Property->Name, Vector4Property->Value));
								break;
							}
							default:
								break;
							}
						}

						/*OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_1I, shaderHandle, "DirectionShadowMap", 0));
						OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_VECTOR3, shaderHandle, "ViewPos",
																		 Engine::Get()->GetWorld()->GetControllerLocation()));
						OutCommands.push_back(
							CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_1I, shaderHandle, "CountPointLight", PointLights.size()));
						OutCommands.push_back(
							CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_1I, shaderHandle, "CountDirectionLight", DirectionLights.size()));
						OutCommands.push_back(
							CommandPool.RequestCommand(RenderCommand::ETypeCommand::UNIFORM_1I, shaderHandle, "CountSpotLight", SpotLights.size()));*/
						/*OutCommands.push_back(MakeUniquePtr<GLCmdSetUniform1i>(shaderHandle, "DirectionShadowMap", 0));
						OutCommands.push_back(
							MakeUniquePtr<GLCmdSetUniformVector3>(shaderHandle, "ViewPos", Engine::Get()->GetWorld()->GetControllerLocation()));
						OutCommands.push_back(MakeUniquePtr<GLCmdSetUniform1i>(shaderHandle, "CountPointLight", PointLights.size()));
						OutCommands.push_back(MakeUniquePtr<GLCmdSetUniform1i>(shaderHandle, "CountDirectionLight", DirectionLights.size()));
						OutCommands.push_back(MakeUniquePtr<GLCmdSetUniform1i>(shaderHandle, "CountSpotLight", SpotLights.size()));*/

						OutCommands.push_back(CommandPool.RequestCommand<GLCmdActivateLayerTexture>());
						OutCommands.push_back(CommandPool.RequestCommand<GLCmdActivateDepthTexture>(ShadowDepth.get()));

						OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniform1i>(shaderHandle, "DirectionShadowMap", 0));
						OutCommands.push_back(
							CommandPool.RequestCommand<GLCmdSetUniformVector3>(shaderHandle, "ViewPos", Engine::Get()->GetWorld()->GetControllerLocation()));
						OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniform1i>(shaderHandle, "CountPointLight", PointLights.size()));
						OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniform1i>(shaderHandle, "CountDirectionLight", DirectionLights.size()));
						OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniform1i>(shaderHandle, "CountSpotLight", SpotLights.size()));

						m_SSBODirectionLight.Bind(0);
						m_SSBOPointLight.Bind(1);
						m_SSBOSpotLight.Bind(2);
						m_SSBOLightSpace.Bind(3);
						glBindBuffer(GL_SHADER_STORAGE_BUFFER, 3);

						/*	OutCommands.push_back(CommandPool.RequestCommand(RenderCommand::ETypeCommand::DRAW_INDEX, Primitive->GetArrayObject()[i],
																			 Primitive->GetIndeces()[i]->size(), []() {}));*/
						// OutCommands.push_back(MakeUniquePtr<GLCmdDrawIndex>(Primitive->GetArrayObject()[i], Primitive->GetIndeces()[i]->size(), []() {}));
						OutCommands.push_back(
							CommandPool.RequestCommand<GLCmdDrawIndex>(Primitive->GetArrayObject()[i], Primitive->GetIndeces()[i]->size(), []() {}));
					}
				}
			}

			void OpenGLRender::ExecuteCommandList(RenderDevice* Device, const DArray<RenderCommand*>& Commands)
			{
				m_ResultScene->Bind();
				ClearBuffersScreen();
				for (auto& Command : Commands)
				{
					Command->Execute(Device);
				}
				m_ResultScene->UnBind();
			}

			void OpenGLRender::CollectDataFromProxy(const DArray<LightProxy*>& Lights, DArray<SimplyDirectionLightProxy>& OutDirectionLights,
													DArray<SimplyPointLightProxy>& OutPointLights, DArray<SimpleSpotLightProxy>& OutSpotLights)
			{
				for (int64 i = 0; i < Lights.size(); i++)
				{
					DArray<GLuint64> Handles;
					auto* ShadowData = FindLightShadowData(Lights[i]->GetTypeLight(), Lights[i]->GetID());

					switch (Lights[i]->GetTypeLight())
					{
					case ETypeLight::DIRECTION_LIGHT:
					{
						DirectionLightProxy* Proxy = dynamic_cast<DirectionLightProxy*>(Lights[i]);
						SimplyDirectionLightProxy SimplePointLight = {Proxy->GetColor(), Proxy->GetIntencity(), Proxy->GetDirection(),
																	  ShadowData ? ShadowData->Layer : 0, 0};
						OutDirectionLights.emplace_back(SimplePointLight);
						break;
					}
					case ETypeLight::POINT_LIGHT:
					{
						PointLightProxy* Proxy = dynamic_cast<PointLightProxy*>(Lights[i]);
						SimplyPointLightProxy SimplePointLight = {Proxy->GetColor(),	Proxy->GetIntencity(), Proxy->GetLocation(),
																  Proxy->GetConstant(), Proxy->GetLinear(),	   Proxy->GetQuadratic()};
						OutPointLights.emplace_back(SimplePointLight);
						break;
					}
					case ETypeLight::SPOTLIGHT:
					{
						SpotLightProxy* Proxy = dynamic_cast<SpotLightProxy*>(Lights[i]);
						SimpleSpotLightProxy SimpleSpotLight = {
							Proxy->GetColor(),	   Proxy->GetDirection(), Proxy->GetLocation(),	   ShadowData ? ShadowData->Layer : -1,
							Proxy->GetIntencity(), Proxy->GetCutOff(),	  Proxy->GetOuterCutOff(), Proxy->GetConstant(),
							Proxy->GetLinear(),	   Proxy->GetQuadratic()};
						OutSpotLights.emplace_back(SimpleSpotLight);
						break;
					}
					default:
						break;
					}
				}

				m_SSBODirectionLight.CreaterBuffer(OutDirectionLights.data(), OutDirectionLights.size(), sizeof(SimplyDirectionLightProxy),
												   ETypeStorageDraw::STREAM);
				m_SSBOPointLight.CreaterBuffer(OutPointLights.data(), OutPointLights.size(), sizeof(SimplyPointLightProxy), ETypeStorageDraw::STREAM);
				m_SSBOSpotLight.CreaterBuffer(OutSpotLights.data(), OutSpotLights.size(), sizeof(SimpleSpotLightProxy), ETypeStorageDraw::STREAM);
				m_SSBOLightSpace.CreaterBuffer(LightsMatrix.data(), LightsMatrix.size(), sizeof(FMatrix4x4), ETypeStorageDraw::STREAM);
			}

			void OpenGLRender::BuildShadowDepthCommandList(RenderDevice* Device, const DArray<LightProxy*>& Lights,
														   const DArray<CoreEngine::PrimitiveProxy*>& Primitives, DArray<RenderCommand*>& OutCommands)
			{
				for (auto* Light : Lights)
				{
					if (Light->GetTypeLight() == ETypeLight::POINT_LIGHT) continue;
					switch (Light->GetTypeLight())
					{
					case ETypeLight::DIRECTION_LIGHT:
						BuildDirectionLightShadowDepthCommandList(Device, Light, Primitives, LightsMatrix, OutCommands);
						break;
					case ETypeLight::SPOTLIGHT:
						BuildSpotLightShadowDepthCommandList(Device, Light, Primitives, LightsMatrix, OutCommands);
						break;
					default:
						break;
					}
					m_CurrentShadowFramebufferLayer++;
				}
			}

			void OpenGLRender::BuildSpotLightShadowDepthCommandList(RenderDevice* Device, LightProxy* Lights,
																	const DArray<CoreEngine::PrimitiveProxy*>& Primitives, DArray<FMatrix4x4>& LightSpaces,
																	DArray<RenderCommand*>& OutCommands)
			{
				auto* SpotLight = dynamic_cast<SpotLightProxy*>(Lights);
				FMatrix4x4 LightSpace(1);

				FMatrix4x4 ProjectionLight = Math::CreatePerspectiveMatrix((SpotLight->GetOuterCutOff() * 8), ShadowDepth->GetSpecifiction().Width,
																		   ShadowDepth->GetSpecifiction().Height, 0.1, 10001);
				FVector DirectionLight = (SpotLight->GetDirection());

				FMatrix4x4 ViewLight =
					Math::CreateMatrixLookAt(SpotLight->GetLocation(), SpotLight->GetLocation() + SpotLight->GetDirection() * 100, FVector::UpVector);
				LightSpace = ProjectionLight * ViewLight;
				LightSpaces.push_back(LightSpace);
				OutCommands.push_back(CommandPool.RequestCommand<GLCmdBindFramebufferArray>(ShadowDepth.get(), m_CurrentShadowFramebufferLayer));
				OutCommands.push_back(CommandPool.RequestCommand<GLCmdClearColorAndDepth>());
				OutCommands.push_back(CommandPool.RequestCommand<CmdBindShaderProgram>(m_ShaderShadow->GetHandle(), []() {}));
				OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(m_ShaderShadow->GetHandle(), "LightSpaceMat", LightSpace));

				for (auto* Proxy : Primitives)
				{
					if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Proxy))
					{
						for (size_t i = 0; i < StaticProxy->GetIndeces().size(); i++)
						{
							// StaticProxy->GetArrayObject()[i]->Bind();

							using namespace CoreEngine::Render;

							// m_ShaderShadow->SetUniformMatrix4x4("LightSpaceMat", LightSpace, false);
							// m_ShaderShadow->SetUniformMatrix4x4("ModelMat", StaticProxy->GetTransformMatrix(), false);
							OutCommands.push_back(CommandPool.RequestCommand<GLCmdSetUniformMatrix4x4>(m_ShaderShadow->GetHandle(), "ModelMat",
																									   StaticProxy->GetTransformMatrix()));

							OutCommands.push_back(
								CommandPool.RequestCommand<GLCmdDrawIndex>(StaticProxy->GetArrayObject()[i], StaticProxy->GetIndeces()[i]->size(), []() {}));
						}
					}
				}

				// OutCommands.push_back(CommandPool.RequestCommand<GLCmdBindFramebufferArray>(ShadowDepth.get(), m_CurrentShadowFramebufferLayer));
			}

			void OpenGLRender::ExecuteShadowDepthCommand(RenderDevice* Device, const DArray<LightProxy*>& Lights, const DArray<RenderCommand*>& Commands)
			{
				// Shadow depth
				uint32 SumSize = 0;
				for (auto& It : m_LightsFramebuffer)
				{
					SumSize += It.second.size();
				}
				if (Lights.size() != SumSize)
				{
					m_LightsFramebuffer.clear();
				}

				uint32 LayerIndex = 0;
				// m_ShaderShadow->Bind();
				ShadowDepth->Bind();
				glCullFace(GL_FRONT);
				// for (auto* Light : Lights)
				//{
				//	if (Light->GetTypeLight() == ETypeLight::POINT_LIGHT) continue;
				//	auto* CreateBuffer = FindLightShadowData(Light->GetTypeLight(), Light->GetID());

				//	if (!CreateBuffer)
				//	{
				//		LightShadowData LightData;
				//		LightData.Layer = LayerIndex;
				//		LightData.IDLight = Light->GetID();
				//		CreateBuffer = &m_LightsFramebuffer[Light->GetTypeLight()].emplace_back(LightData);
				//	}
				//	ShadowDepth->BindDepthLayar(LayerIndex);
				//	ClearBuffersScreen();

				//	switch (Light->GetTypeLight())
				//	{
				//	case ETypeLight::DIRECTION_LIGHT:
				//		LightsMatrix.push_back(DrawDirectionLightShadowBuffer(Light, Primitives));
				//		break;
				//	case ETypeLight::SPOTLIGHT:
				//		LightsMatrix.push_back(DrawSpotLightShadowBuffer(Light, Primitives));
				//	default:
				//		break;
				//	}

				//	LayerIndex++;
				//	// CreateBuffer->ShadowDepth->UnBind();
				//}
				for (auto* Light : Lights)
				{
					if (Light->GetTypeLight() == ETypeLight::POINT_LIGHT) continue;

					auto* CreateBuffer = FindLightShadowData(Light->GetTypeLight(), Light->GetID());

					if (!CreateBuffer)
					{
						LightShadowData LightData;
						LightData.Layer = LayerIndex;
						LightData.IDLight = Light->GetID();
						CreateBuffer = &m_LightsFramebuffer[Light->GetTypeLight()].emplace_back(LightData);
					}
					ShadowDepth->ActivateDepthTexture();
					ShadowDepth->BindDepthLayar(LayerIndex);
					for (auto& Command : m_ShadowDepthListCommand)
					{
						Command->Execute(Device);
					}
					LayerIndex++;
				}
				ShadowDepth->UnBind();
				glCullFace(GL_BACK);
			}

		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine

namespace CoreEngine::Render::OpenGL
{
	void RenderCommandPool::ReturnCommand(RenderCommand* Command)
	{
		auto& ArrCeil = m_CommandPool.find(typeid(*Command));
		if (ArrCeil == m_CommandPool.end()) return;

		for (auto& el : ArrCeil->second)
		{
			if (el.IsUsed && Command == el.Command.get() && el.Type == typeid(*Command))
			{
				el.IsUsed = false;
				return;
			}
		}
	}

	void RenderCommandPool::ClearCommand()
	{
		for (auto& Pools : m_CommandPool)
		{
			for (auto& CeilCommand : Pools.second)
			{
				CeilCommand.IsUsed = false;
			}
		}
	}

} // namespace CoreEngine::Render::OpenGL
