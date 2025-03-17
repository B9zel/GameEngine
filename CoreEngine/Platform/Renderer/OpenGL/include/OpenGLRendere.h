#pragma once
#include <Render/includes/Render.h>
#include <glad/glad.h>
#include <Math/includes/Matrix.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLRender : public Render
			{
			public:

				virtual void ClearBuffersScreen() override;
				virtual void SetViewProjectionMatrix(const FMatrix4x4& View, const FMatrix4x4& Projection) override;

			protected:

				virtual void RenderStaticMeshProxy(const StaticMeshProxy* Proxy) override;
				virtual void RenderProxy(const PrimitiveProxy* Proxy) override;

			private:

				FMatrix4x4 m_View;
				FMatrix4x4 m_Projection;
			};
		}
	}
}