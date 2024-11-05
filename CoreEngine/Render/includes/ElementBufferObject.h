#pragma once
#include <Render/includes/Enums/TypeDraw.h>
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/VertextBufferObject.h>


namespace CoreEngine
{
	namespace Render
	{
		class ElementBufferObject
		{
		public:

			virtual ~ElementBufferObject() = default;

			virtual void CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeDraw& typeDraw, const VertexBufferObject& buffObj) = 0;
			virtual void DeleteBuffer() = 0;

			virtual bool SetData(const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind = true) = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;

		};
	}
}