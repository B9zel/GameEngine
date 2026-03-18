#include "Platform/Renderer/OpenGL/include/OpenGLRenderCommand.h"
#include "Render/includes/VertexArrayObject.h"
#include "glad/glad.h"


namespace CoreEngine::Render::OpenGL
{
	void GLCmdDrawIndex::Execute(RenderDevice* Devise)
	{
		const Render::VertexArrayObject* ArrayObject = Devise->GetVAOID(VAOHand);
		if (!ArrayObject)
		{
			EG_LOG(RenderCommandLog, ELevelLog::ERROR, "Array object isn't valid");
			return;
		}
		ArrayObject->Bind();
		glDrawElements(GL_TRIANGLES, CountIndex, GL_UNSIGNED_INT, nullptr);
	}

	RenderCommand::ETypeCommand CoreEngine::Render::OpenGL::GLCmdDrawIndex::GetType() const
	{
		return ETypeCommand::DRAW_INDEX;
	}
} // namespace CoreEngine::Render::OpenGL