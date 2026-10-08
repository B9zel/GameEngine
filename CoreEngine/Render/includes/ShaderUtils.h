#pragma once

#include <Core/includes/Base.h>

struct SourceShader
{
public:

	SourceShader() = default;
	SourceShader(const SourceShader& Other);
	SourceShader(SourceShader&& Other);


	bool operator==(const SourceShader& Other) const;
	bool operator!=(const SourceShader& Other) const;

	SourceShader& operator=(const SourceShader& Other);
	SourceShader& operator=(SourceShader&& Other) noexcept;

public:

	String VertexShader;
	String FragmentShader;
};
