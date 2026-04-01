#pragma once
#include <Render/includes/Shader.h>
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>
#include <Math/includes/Matrix.h>
#include <Math/includes/Vector.h>
#include <Render/includes/RenderHardwareInterface.h>
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
				virtual ~OpenGLShader();
				OpenGLShader(OpenGLShader&& otherShader) noexcept;
				OpenGLShader(RenderDevice* Device, const String& vertexShader, const String& fragmentShader);

				OpenGLShader(const OpenGLShader&) = delete;
				OpenGLShader& operator=(const OpenGLShader&) = delete;

				OpenGLShader& operator=(OpenGLShader&& otherShder) noexcept;

				virtual bool CompileShader(RenderDevice* Device, const String& vertexShader, const String& fragmentShader) override;
				virtual bool CompileShader(RenderDevice* Device, const StringView vertexShader, const StringView fragmentShader) override;
				virtual const DArray<String>& GetNamesOfTexture() const;
				virtual bool GetIsCompile() override;
				virtual bool GetHasAllMatrix() override;
				virtual RHI::ShaderHandle GetHandle() const override;

				virtual void Bind(RenderDevice* Device) const override;
				virtual void UnBind(RenderDevice* Device) const override;

				virtual bool SetUniformMatrix4x4(RenderDevice* Device, const String& nameParam, const FMatrix4x4& matrix, bool isBindShader = true) override;
				virtual bool SetUniform1i(RenderDevice* Device, const String& nameParam, const int32 value, bool isEnableBind = true) override;
				virtual bool SetUniform1ui(RenderDevice* Device, const String& nameParam, const uint32 value, bool isEnableBind = true) override;
				virtual bool SetUniformFloat(RenderDevice* Device, const String& nameParam, float value, bool isEnableBind = true) override;
				virtual bool SetUniformVec4(RenderDevice* Device, const String& nameParam, const FVector4& vec, bool isEnableBind = true) override;
				virtual bool SetUniformVec2(RenderDevice* Device, const String& nameParam, const FVector2& vec, bool isEnableBind = true) override;
				virtual bool SetUniformVec3(RenderDevice* Device, const String& nameParam, const FVector& vec, bool isEnableBind = true) override;

				bool HasUniformLocation(const char* nameParam);
				int32 GetUniformLocation(const char* nameParam);

			private:

				bool GetCachedLocationParam(const String& Key, int32& outLocation);
				void AnalysisTextureShader(const String& vertex, const String& fragment);
				void AnalysisMatrix4(const String& vertex);
				bool IsInComment(const String& str, const size_t PosTarget);

			private:

				HashTableMap<String, int32> cachedParameters;
				DArray<String> m_NameTextures;


				uint32 m_ID;
				bool m_IsCompile;
				bool m_HasAllMatrix; // Has Model, View, Projection matrix

				/////////////////////////////////

				RHI::ShaderHandle Handle;
			};
		}
	}
}