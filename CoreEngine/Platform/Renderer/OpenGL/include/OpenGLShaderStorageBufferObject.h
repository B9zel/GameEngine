#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Render/includes/ShaderStorageBufferObject.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLShaderStorageBufferObject : public ShaderStorageBufferObject
			{
			public:

				OpenGLShaderStorageBufferObject();

			public:

				virtual void GenerationBuffer() override;
				virtual void CreaterBuffer(const void* vertexArr, const uint32 sizeArr, const uint32 BytesOfData, const ETypeDraw& typeDraw) override;

				virtual void DeleteBuffer() override;

				virtual bool SetData(const int32 beginFillData, const uint32 sizeOfData, const uint32 BytesOfData, const void* data, const bool isBind = true) override;

				virtual void Bind(const uint32 IndexOfBuffer) override;
				virtual void UnBind() override;

			private:

				bool m_IsCreated;
				uint32 m_BufferID;
			};
		}
	}
}