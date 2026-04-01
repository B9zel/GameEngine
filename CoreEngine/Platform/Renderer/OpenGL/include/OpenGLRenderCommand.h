#pragma once
#include "Render/includes/RenderCommand.h"

namespace CoreEngine::Render
{
	class VertexArrayObject;
}

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

} // namespace CoreEngine::Render::OpenGL
