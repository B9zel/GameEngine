#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>
#include <Core/includes/Base.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/StaticMeshProxy.h>
#include <Core/includes/LightProxy.h>
#include <Core/includes/Engine.h>
#include <Core/includes/World.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <iostream>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLRender::OpenGLRender()
			{
				m_SSBODirectionLight.CreaterBuffer(nullptr, 0, sizeof(SimplyDirectionLightProxy), ETypeDraw::STREAM);
				m_SSBOPointLight.CreaterBuffer(nullptr, 0, sizeof(SimplyPointLightProxy), ETypeDraw::STREAM);
				m_SSBOSpotLight.CreaterBuffer(nullptr, 0, sizeof(SimpleSpotLightProxy), ETypeDraw::STREAM);
			}

			void OpenGLRender::RenderStaticMeshProxy(const StaticMeshProxy* Proxy, const DArray<LightProxy*>& Lights)
			{
				DArray<SimplyDirectionLightProxy> DirectionLights;
				DArray<SimplyPointLightProxy> PointLights;
				DArray<SimpleSpotLightProxy> SpotLights;
			
				for (int64 i = 0; i < Lights.size(); i++)
				{
					switch (Lights[i]->GetTypeLight())
					{
					case ETypeLight::DIRECTION_LIGHT:
					{
						DirectionLightProxy* Proxy = dynamic_cast<DirectionLightProxy*>(Lights[i]);
						SimplyDirectionLightProxy SimplePointLight = { Proxy->GetColor(), Proxy->GetIntencity(), Proxy->GetDirection() };
						DirectionLights.emplace_back(SimplePointLight);
						break;
					}
					case ETypeLight::POINT_LIGHT:
					{
						PointLightProxy* Proxy = dynamic_cast<PointLightProxy*>(Lights[i]);
						SimplyPointLightProxy SimplePointLight = { Proxy->GetColor(), Proxy->GetIntencity(), Proxy->GetLocation(), Proxy->GetConstant(),
																	Proxy->GetLinear(), Proxy->GetQuadratic() };
						PointLights.emplace_back(SimplePointLight);
						break;
					}
					case ETypeLight::SPOTLIGHT:
					{
						SpotLightProxy* Proxy = dynamic_cast<SpotLightProxy*>(Lights[i]);
						SimpleSpotLightProxy SimpleSpotLight = { Proxy->GetColor(),  Proxy->GetDirection(), Proxy->GetLocation(),  Proxy->GetIntencity(),Proxy->GetCutOff(), Proxy->GetOuterCutOff(), Proxy->GetConstant(),
																	Proxy->GetLinear(), Proxy->GetQuadratic() };
						SpotLights.emplace_back(SimpleSpotLight);
						break;
					}
					default:
						break;
					}
				}
				m_SSBODirectionLight.CreaterBuffer(DirectionLights.data(), DirectionLights.size(), sizeof(SimplyDirectionLightProxy), ETypeDraw::STREAM);

				m_SSBOPointLight.CreaterBuffer(PointLights.data(), PointLights.size(), sizeof(SimplyPointLightProxy), ETypeDraw::STREAM);
				m_SSBOSpotLight.CreaterBuffer(SpotLights.data(), SpotLights.size(), sizeof(SimpleSpotLightProxy), ETypeDraw::STREAM);

				//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, alignof(PointLightProxy));
				//glDisable(GL_BLEND);
			
				int test = 0;
				for (size_t i = 0; i < Proxy->GetIndeces().size(); i++)
				{
					for (auto& el : Proxy->GetShaders())
					{

						//if (test++ == 0) continue;
						auto* shader = el.first;
						auto* vertexArray = Proxy->GetArrayObject()[i];

						vertexArray->Bind();
						shader->Bind();

						auto& Textures = Proxy->GetTextures();

						for (uint32 j = 0; j < Textures.size(); j++)
						{
							Textures[j]->Bind(j);

							const DArray<String>& NamesTexture = shader->GetNamesOfTexture();
							if (!NamesTexture.empty())
							{
								if (j >= NamesTexture.size()) continue;

								shader->SetUniform1i(NamesTexture[j], j, false);
							}
						}


						if (shader->GetHasAllMatrix())
						{
							FVector4 a = m_Projection * m_View * Proxy->GetTransform().ToMatrix() * FVector4(1, 1, 1, 1).vector;
							//FMatrix4x4 b = m_Projection * m_View * Proxy->GetTransform().ToMatrix();

							shader->SetUniformMatrix4x4("Model", Proxy->GetTransform().ToMatrix(), false);

							shader->SetUniformMatrix4x4("View", m_View, false);
							shader->SetUniformMatrix4x4("Projection", m_Projection, false);
						}

						shader->SetUniformVec3("ViewPos", Engine::Get()->GetWorld()->GetControllerLocation(), false);
						shader->SetUniform1i("CountPointLight", PointLights.size(), false);
						shader->SetUniform1i("CountDirectionLight", DirectionLights.size(), false);
						shader->SetUniform1i("CountSpotLight", SpotLights.size(), false);
						/*if (Proxy->GetUUID()->IsValid())
						{
							shader->SetUniform1i("id", Proxy->GetUUID()->GetID(), false);

						}
						else
						{

							shader->SetUniform1i("id", 0, false);
						}*/
						/*shader->SetUniform1i("texture", 0, false);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, 3);*/

						//int b;
						//glGetUniformiv(3, glGetUniformLocation(3, "CountSpotLight"), &b);
						//EG_LOG(CORE, ELevelLog::INFO, "{0}", b);
						if (!Proxy->GetLocationLights().empty())
						{
							shader->SetUniformVec3("PosLight", Proxy->GetLocationLights()[0], false);
						}


						int ebo;
						m_SSBODirectionLight.Bind(0);
						m_SSBOPointLight.Bind(1);
						m_SSBOSpotLight.Bind(2);
						glBindBuffer(GL_SHADER_STORAGE_BUFFER, 3);
						//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
						//float* dat = nullptr;
						//dat = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 60, GL_MAP_READ_BIT);

						//if (dat)
						//{
						//	for (int i = 0; i < 60 / sizeof(float); i += 3)
						//	{
						//		EG_LOG(CORE, ELevelLog::INFO, "{0}, {1}, {2}", dat[i], dat[i + 1], dat[i + 2]);
						//	}
						//}

						/*unsigned int a;
						int locat = glGetUniformLocation(3, "ID");
						glGetUniformuiv(3, locat, &a);
						EG_LOG(CORE, ELevelLog::INFO, a);*/
						//	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 2);
						//	glGetIntegerv(GL_SHADER_STORAGE_BUFFER, &ebo);
						//EG_LOG(CORE, ELevelLog::INFO, ebo);
						//void* d = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
						//if (d != nullptr)
						//{
						//	PointLightProxy* l = reinterpret_cast<PointLightProxy*>(d);
						//	for (size_t i = 0; i < 32; i++)
						//	{

						//		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, *((double*)l + i * sizeof(double)));
						//	}

						//}
						glDrawElements(GL_TRIANGLES, Proxy->GetIndeces()[i]->size(), GL_UNSIGNED_INT, 0);

						shader->UnBind();
						vertexArray->UnBind();

						glBindTexture(GL_TEXTURE_2D, 0);
					}
					test = 0;
				}
				//glEnable(GL_BLEND);
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

			void OpenGLRender::RenderProxy(const PrimitiveProxy* Proxy)
			{
				for (auto& el : Proxy->GetShaders())
				{
					auto* shader = el.first;
					auto* vertexArray = el.second.first;

					shader->Bind();
					vertexArray->Bind();


					auto& Textures = Proxy->GetTextures();

					for (uint32 i = 0; i < Textures.size(); i++)
					{
						Textures[i]->Bind(i);

						const DArray<String>& NamesTexture = shader->GetNamesOfTexture();
						if (!NamesTexture.empty())
						{
							if (i >= NamesTexture.size()) continue;

							shader->SetUniform1i(NamesTexture[i], i, false);
						}
					}


					if (shader->GetHasAllMatrix())
					{
						FMatrix4x4 a = m_Projection * m_View * Proxy->GetTransform().ToMatrix();

						shader->SetUniformMatrix4x4("Model", Proxy->GetTransform().ToMatrix(), false);
						shader->SetUniformMatrix4x4("View", m_View, false);
						shader->SetUniformMatrix4x4("Projection", m_Projection, false);
					}

					glDrawArrays(GL_TRIANGLES, 0, Proxy->CountVertex / 3);


					glDrawElements(GL_TRIANGLES, Proxy->CountIndeces, GL_UNSIGNED_INT, 0);
					el.first->UnBind();
					vertexArray->UnBind();

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

		}
	}
}
