#pragma once
#include <Render/includes/Shader.h>
#include <Core/includes/Log.h>

#include <glad/glad.h>
#include <gl/GL.h>


namespace CoreEngine
{
	namespace Render
	{
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

			void SetUniformFloat(const char* nameParam, float value);

		private:

			unsigned int m_ID;
			bool m_IsCompile;
		};

	}


}