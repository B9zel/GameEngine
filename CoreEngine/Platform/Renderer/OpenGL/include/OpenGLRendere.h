#pragma once
#include <Render/includes/Render.h>
#include <glad/glad.h>
#include <Math/includes/Matrix.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShaderStorageBufferObject.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLRender : public Render
			{
			public:

				OpenGLRender();

			public:

				virtual void ClearBuffersScreen() override;
				virtual void SetViewProjectionMatrix(const FMatrix4x4& View, const FMatrix4x4& Projection) override;

			protected:

				virtual void RenderStaticMeshProxy(const StaticMeshProxy* Proxy, const DArray<LightProxy*>& Lights) override;
				virtual void RenderProxy(const PrimitiveProxy* Proxy) override;

			private:

				FMatrix4x4 m_View;
				FMatrix4x4 m_Projection;
				OpenGLShaderStorageBufferObject m_SSBODirectionLight;
				OpenGLShaderStorageBufferObject m_SSBOPointLight;
				OpenGLShaderStorageBufferObject m_SSBOSpotLight;
			};
		}
	}
}