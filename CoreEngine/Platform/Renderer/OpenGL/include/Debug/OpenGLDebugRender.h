#pragma once
#include <Render/includes/Debug/DebugRender.h>
#include <Render/includes/RenderHardwareInterface.h>

namespace CoreEngine::Render
{
	class RenderDevice;
}

namespace CoreEngine::Render::OpenGL
{
	class OpenGLDebugRender : public DebugRender
	{
	public:

		explicit OpenGLDebugRender(RenderDevice& Device);
		~OpenGLDebugRender() override;

		OpenGLDebugRender(const OpenGLDebugRender&) = delete;
		OpenGLDebugRender& operator=(const OpenGLDebugRender&) = delete;

		virtual void Flush(const FMatrix4x4& view, const FMatrix4x4& projection) override;

	private:

		RenderDevice& m_Device;
		RHI::BufferHandle m_VBO;
		RHI::ShaderHandle m_Shader;
		// RenderDevice::CreateVAO currently requires an index buffer.
		uint32 m_VAO = 0;
	};
} // namespace CoreEngine::Render::OpenGL
