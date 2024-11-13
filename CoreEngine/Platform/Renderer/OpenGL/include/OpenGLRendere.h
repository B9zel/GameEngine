#pragma once
#include <Render/includes/Render.h>
#include <glad/glad.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLRender : public Render
			{
			protected:

				virtual void RenderStaticMeshProxy(const StaticMehsProxy* Proxy) override;
				virtual void RenderProxy(const PrimitiveProxy* Proxy) override;
			};
		}
	}
}