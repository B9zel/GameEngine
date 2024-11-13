#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Matrix.h>


namespace CoreEngine
{
	namespace Render
	{
		class Shader
		{
		public:

			/*
			* @return First string store the vertex shader
			* Second string store the fragment shader
			*/
			static Pair<String, String> LoadShader(const char* pathToShader);

			virtual bool CompileShader(const String& vertexShader, const String& fragmentShader) = 0;

			virtual bool GetIsCompile() = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;

			virtual bool SetUniformMatrix4x4(const String& nameParam,const FMatrix4x4& matrix) = 0;
			virtual bool SetUniform1i(const String& nameParam, const int32 value, bool isEnableBind = true) = 0;
			virtual const DArray<String>& GetNamesOfTexture() const = 0;
		};

	}
}