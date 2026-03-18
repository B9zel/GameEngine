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
		uint32 CountIndex;

	public:

		virtual void Execute(RenderDevice* Devise) override;
		virtual ETypeCommand GetType() const override;
	};

} // namespace CoreEngine::Render::OpenGL
