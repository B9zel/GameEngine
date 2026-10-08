#pragma once
#include "Render/includes/RenderCommand.h"

namespace CoreEngine::Render
{
	namespace OpenGL
	{
		class OpenGLFramebufferArray;
	}

	class VertexArrayObject;
} // namespace CoreEngine::Render

namespace CoreEngine::Render::OpenGL
{
	struct GLCmdDrawIndex : public RenderCommand
	{
		RHI::HandleVAO VAOHand;
		uint64 CountIndex;

		using PredParams = void (*)();
		// Call after Execute
		FunctionPtr<void()> Predicate;

	public:

		GLCmdDrawIndex(const RHI::HandleVAO& Handle, const uint64 CountIndex, PredParams Pred) : VAOHand(Handle), CountIndex(CountIndex), Predicate(Pred)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdSetUniform1i : public RenderCommand
	{
		RHI::ShaderHandle Handle;
		String NameParam;
		int64 Value;

	public:

		GLCmdSetUniform1i(const RHI::ShaderHandle Handle, const String NameOfParam, int64 Value) : Handle(Handle), NameParam(NameOfParam), Value(Value)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdSetUniform1f : public RenderCommand
	{
		RHI::ShaderHandle Handle;
		String NameParam;
		float Value;

	public:

		GLCmdSetUniform1f(const RHI::ShaderHandle Handle, const String NameOfParam, float Value) : Handle(Handle), NameParam(NameOfParam), Value(Value)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdSetUniformMatrix4x4 : public RenderCommand
	{
		RHI::ShaderHandle Handle;
		String NameParam;
		FMatrix4x4 Value;

	public:

		GLCmdSetUniformMatrix4x4(const RHI::ShaderHandle Handle, const StringView NameOfParam, FMatrix4x4 Value)
			: Handle(Handle), NameParam(NameOfParam), Value(Value)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdSetUniformVector3 : public RenderCommand
	{
		RHI::ShaderHandle Handle;
		String NameParam;
		FVector Value;

	public:

		GLCmdSetUniformVector3(const RHI::ShaderHandle Handle, const StringView NameOfParam, FVector Value)
			: Handle(Handle), NameParam(NameOfParam), Value(Value)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdBindFramebufferArray : public RenderCommand
	{
		CoreEngine::Render::OpenGL::OpenGLFramebufferArray* Framebuffer; // set handle late
		uint32 Layer = 0;

	public:

		GLCmdBindFramebufferArray(CoreEngine::Render::OpenGL::OpenGLFramebufferArray* Framebuffer, const uint32 Layer) : Framebuffer(Framebuffer), Layer(Layer)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdClearColorAndDepth : public RenderCommand
	{
	public:

		GLCmdClearColorAndDepth() = default;

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdActivateDepthTexture : public RenderCommand
	{
		CoreEngine::Render::OpenGL::OpenGLFramebufferArray* Framebuffer;

	public:

		GLCmdActivateDepthTexture(CoreEngine::Render::OpenGL::OpenGLFramebufferArray* Framebuffer) : Framebuffer(Framebuffer)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

	struct GLCmdActivateLayerTexture : public RenderCommand
	{
		uint32 Layer;

	public:

		GLCmdActivateLayerTexture(uint32 Layer = 0) : Layer(Layer)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

} // namespace CoreEngine::Render::OpenGL
