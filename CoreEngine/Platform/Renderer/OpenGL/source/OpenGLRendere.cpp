#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{

			void OpenGLRender::RenderStaticMeshProxy(const StaticMehsProxy* Proxy)
			{
			//	glDrawArrays(GL_TRIANGLES, 0)
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
					
					glDrawArrays(GL_TRIANGLES, 0, Proxy->CountVertex);

					el.first->UnBind();
					el.second->UnBind();

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			
		}
	}
}
