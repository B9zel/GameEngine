#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include "Platform/Renderer/OpenGL/include/OpenGLRenderCommand.h"
#include "Render/includes/VertexArrayObject.h"
#include "glad/glad.h"

namespace CoreEngine::Render::OpenGL
{
	void GLCmdDrawIndex::Execute(RenderDevice* Devise)
	{
		const uint32 ArrayObject = Devise->GetVAOID(VAOHand);
		if (!ArrayObject)
		{
			EG_LOG(RenderCommandLog, ELevelLog::ERROR, "Array object isn't valid");
			return;
		}
		glBindVertexArray(ArrayObject);
		
		glDrawElements(GL_TRIANGLES, CountIndex, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	RenderCommand::ETypeCommand CoreEngine::Render::OpenGL::GLCmdDrawIndex::GetType() const
	{
		return ETypeCommand::DRAW_INDEX;
	}

	void GLCmdSetUniform1i::Execute(RenderDevice* Devise)
	{
		Devise->SetUniform1i(Handle, NameParam, Value);
	}

	RenderCommand::ETypeCommand GLCmdSetUniform1i::GetType() const
	{
		return ETypeCommand::UNIFORM_1I;
	}

	void GLCmdSetUniformMatrix4x4::Execute(RenderDevice* Devise)
	{
		Devise->SetUniformMatrix4x4(Handle, NameParam, Value);
	}

	RenderCommand::ETypeCommand GLCmdSetUniformMatrix4x4::GetType() const
	{
		return ETypeCommand::UNIFORM_MATRIX_4X4;
	}

	void GLCmdSetUniformVector3::Execute(RenderDevice* Devise)
	{
		Devise->SetUniformVec3(Handle, NameParam, Value);
	}

	RenderCommand::ETypeCommand GLCmdSetUniformVector3::GetType() const
	{
		return ETypeCommand::UNIFORM_VECTOR3;
	}
} // namespace CoreEngine::Render::OpenGL
