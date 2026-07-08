#pragma once

#include <Core/includes/Base.h>

struct SourceShader
{
public:

	bool operator==(const SourceShader& Other) const;
	bool operator!=(const SourceShader& Other) const;

public:

	String VertexShader;
	String FragmentShader;
};
