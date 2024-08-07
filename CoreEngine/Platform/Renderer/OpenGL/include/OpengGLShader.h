#pragma once
#include <Render/includes/Shader.h>
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>
#include <Math/includes/Matrix.h>
#include <Math/includes/Vector.h>
#include <glad/glad.h>
#include <gl/GL.h>


namespace CoreEngine
{
	namespace Render
	{
		DECLARE_LOG_CATEGORY_EXTERN(OPENGL_Shader)

		class OpenGLShader : public Shader
		{
		public:
			
			OpenGLShader();
			OpenGLShader(OpenGLShader&& otherShader) noexcept;
			OpenGLShader(const String& vertexShader, const String& fragmentShader);

			OpenGLShader(const OpenGLShader&) = delete;
			OpenGLShader& operator=(const OpenGLShader&) = delete;

			OpenGLShader& operator=(OpenGLShader&& otherShder) noexcept;

			virtual bool CompileShader(const String& vertexShader, const String& fragmentShader) override;
			virtual bool GetIsCompile() override;

			virtual void Bind() override;
			virtual void UnBind() override;

			virtual bool SetUniformMatrix4x4(const String& nameParam,const FMatrix4x4& matrix) override;
			bool SetUniformFloat(const String& nameParam, float value);
			bool SetUniformVec4(const String& nameParam,const FVector4& vec);
			bool SetUniformVec2(const String& nameParam,const FVector2& vec);
			bool SetUniform1i(const String& nameParam, const int value);

			bool HasUniformLocation(const char* nameParam);
			int GetUniformLocation(const char* nameParam);

		private:

			bool GetCachedLocationParam(const String& Key, int& outLocation);

		private:

			HashTableMap<String, int> cachedParameters;

			unsigned int m_ID;
			bool m_IsCompile;
		};

	}


}