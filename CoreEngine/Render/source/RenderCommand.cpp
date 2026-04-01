#include "Render/includes/RenderCommand.h"
#include "Render/includes/Shader.h"
#include "Render/includes/VertexArrayObject.h"

void CoreEngine::Render::CmdBindShaderProgram::Execute(RenderDevice* Devise)
{
	Devise->BindShader(Handle);
}
