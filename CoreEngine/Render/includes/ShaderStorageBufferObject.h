#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/Enums/TypeDraw.h>

namespace CoreEngine
{
	namespace Render
	{
		class ShaderStorageBufferObject
		{
		public:

			static UniquePtr<ShaderStorageBufferObject> CreateShaderStorageBufferObject();

			virtual void GenerationBuffer() = 0;
			virtual void CreaterBuffer(const void* vertexArr, const uint32 sizeArr, const uint32 BytesOfData, const ETypeDraw& typeDraw) = 0;

			virtual void DeleteBuffer() = 0;

			virtual bool SetData(const int32 beginFillData, const uint32 sizeOfData, const uint32 BytesOfData, const void* data, const bool isBind = true) = 0;

			virtual void Bind(const uint32 IndexOfBuffer) = 0;
			virtual void UnBind() = 0;
		};
	}
}