#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/Render.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Render/includes/RenderDevice.h>
#include <Core/includes/StaticMeshProxy.h>
#include <Core/includes/LightProxy.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRenderDevice.h>
#include <Render/includes/ShaderVariantKey.h>
#include <Render/includes/ShaderCache.h>
#include <Render/includes/ShaderCompiler.h>
#include <Render/includes/Material.h>
#include <Core/includes/AssetManager.h>
#include <Render/includes/Shader.h>
// #include <Render/includes/Framebuffer.h>

namespace CoreEngine
{

	namespace Render
	{
		bool Render::isInit = false;

		Render::Render()
		{
			CORE_UNASSERT(isInit, "Render already create");
			isInit = true;
		}

		UniquePtr<Render> Render::Create()
		{
			UniquePtr<Render> render;
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::OpenGL:
			{
				render = MakeUniquePtr<OpenGL::OpenGLRender>();
				render->m_renderAPI = RendererAPI::CreateAPI();
				render->m_RenderDevice = MakeUniquePtr<OpenGL::OpenGLRenderDevice>();

				break;
			}
			default:
				break;
			}

			return render;
		}

		void Render::RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives, const DArray<LightProxy*>& Lights)
		{
			using namespace CoreEngine::Render;
			for (auto* Primitive : Primitives)
			{
				// Return later !!!!!!!!!!!
				if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Primitive))
				{
					// RenderStaticMeshProxy(StaticProxy, Lights, );
				}
				else
				{
					RenderProxy(Primitive);
				}
			}
		}

		const UniquePtr<RenderDevice>& Render::GetRenderDevice() const
		{
			return m_RenderDevice;
		}

		void Render::PrepareMaterial(RMaterial& Mat)
		{
			if (Mat.GetShaderHandle().IsValid()) return;

			ShaderVariantKey Key;
			Key.m_Hash = Mat.GetShaderAsset()->GetHash();
			Key.ModeRender = Mat.GetModeRender();

			RHI::ShaderHandle shaderHandle;
			if (ShaderCache::TryGetShaderHandle(Key, shaderHandle))
			{
				Mat.SetShaderHandle(shaderHandle);
				return;
			}
			CompileShaderSource Source = ShaderCompiler::Get().Build(*Mat.GetShaderAsset(), Key);

			Shader* shader = AssetManager::Get().LoadShader(Source.VertexShader, Source.FragmentShader);
			Mat.SetShaderHandle(shader->GetHandle());

			ShaderCache::AddShader(Key, shader);
		}

	} // namespace Render
} // namespace CoreEngine
