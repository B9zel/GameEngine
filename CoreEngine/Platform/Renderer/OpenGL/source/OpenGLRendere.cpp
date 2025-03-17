#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <iostream>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{

			void OpenGLRender::RenderStaticMeshProxy(const StaticMeshProxy* Proxy)
			{
				//	glDrawArrays(GL_TRIANGLES, 0)
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
					auto* vertexArray = el.second;

					shader->Bind();
					el.second->Bind();

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
						FMatrix4x4 a = m_View * Proxy->GetTransform().ToMatrix();
						//FMatrix4x4 b = m_Projection * m_View * Proxy->GetTransform().ToMatrix();

						shader->SetUniformMatrix4x4("Model", Proxy->GetTransform().ToMatrix(), false);
						/*for (size_t i = 0; i < 4; i++)
						{
							for (size_t j = 0; j < 4; j++)
							{
								std::cout << a[j][i] << " ";
							}
							std::cout << std::endl;
						}*/
						//std::cout << std::endl << std::endl;
						shader->SetUniformMatrix4x4("View", m_View, false);
						shader->SetUniformMatrix4x4("Projection", m_Projection, false);
					}

					glDrawArrays(GL_TRIANGLES, 0, Proxy->CountVertex / 3);

					el.first->UnBind();
					el.second->UnBind();

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

		}
	}
}
