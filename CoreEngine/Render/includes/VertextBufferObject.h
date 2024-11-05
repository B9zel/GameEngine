#pragma once
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Core/includes/Platform.h>


namespace CoreEngine
{
	namespace Render
	{

		class VertexBufferObject
		{
		public:

			virtual void CreaterBuffer(const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) = 0;

			virtual void DeleteBuffer() = 0;

			virtual bool SetData(const int32 beginFillData, const uint32 sizeOfData, const void* data, const bool isBind = true) = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;
		};
	}

}