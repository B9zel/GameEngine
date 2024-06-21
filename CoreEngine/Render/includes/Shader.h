#pragma once
#include <Core/includes/Base.h>



namespace CoreEngine
{
	namespace Render
	{
		class Shader
		{
		public:

			virtual bool CompileShader(const String& vertexShader, const String& fragmentShader) = 0;

			virtual bool GetIsCompile() = 0;

			virtual void Bind() = 0;
			virtual void UnBind() = 0;
		};

	}
}