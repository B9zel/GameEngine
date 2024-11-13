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
		namespace OpenGL
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
				virtual const DArray<String>& GetNamesOfTexture() const;
				virtual bool GetIsCompile() override;

				virtual void Bind() const override;
				virtual void UnBind() const override;

				virtual bool SetUniformMatrix4x4(const String& nameParam, const FMatrix4x4& matrix) override;
				virtual bool SetUniform1i(const String& nameParam, const int32 value, bool isEnableBind = true) override;
				bool SetUniformFloat(const String& nameParam, float value);
				bool SetUniformVec4(const String& nameParam, const FVector4& vec);
				bool SetUniformVec2(const String& nameParam, const FVector2& vec);

				bool HasUniformLocation(const char* nameParam);
				int32 GetUniformLocation(const char* nameParam);

			private:

				bool GetCachedLocationParam(const String& Key, int32& outLocation);
				void AnalysisShader(const String& vertex, const String& fragment);

			private:

				HashTableMap<String, int32> cachedParameters;
				DArray<String> m_NameTextures;

				uint32 m_ID;
				bool m_IsCompile;
			};
		}
	}
}