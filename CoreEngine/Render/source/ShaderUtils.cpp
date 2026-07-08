#include <Render/includes/ShaderUtils.h>

bool SourceShader::operator==(const SourceShader& Other) const
{
	return VertexShader == Other.VertexShader && FragmentShader == Other.FragmentShader;
}

bool SourceShader::operator!=(const SourceShader& Other) const
{
	return !this->operator==(Other);
}
