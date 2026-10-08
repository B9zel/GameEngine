#include <Render/includes/ShaderUtils.h>

SourceShader::SourceShader(const SourceShader& Other)
{
	this->operator=(Other);
}

SourceShader::SourceShader(SourceShader&& Other)
{
	this->operator=(std::move(Other));
}

bool SourceShader::operator==(const SourceShader& Other) const
{
	return VertexShader == Other.VertexShader && FragmentShader == Other.FragmentShader;
}

bool SourceShader::operator!=(const SourceShader& Other) const
{
	return !this->operator==(Other);
}

SourceShader& SourceShader::operator=(const SourceShader& Other)
{
	VertexShader = Other.VertexShader;
	FragmentShader = Other.FragmentShader;

	return *this;
}

SourceShader& SourceShader::operator=(SourceShader&& Other) noexcept
{
	VertexShader = std::move(Other.VertexShader);
	FragmentShader = std::move(Other.FragmentShader);

	return *this;
}
