#pragma once
#include <Render/includes/Enums/TypeDraw.h>
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/VertextBufferObject.h>
#include <Render/includes/VertexArrayObject.h>

namespace CoreEngine
{
	namespace Render
	{
		class ElementBufferObject
		{
		public:

			static UniquePtr<ElementBufferObject> CreateElementBuffer();

			virtual ~ElementBufferObject() = default;

			virtual void CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeStorageDraw& typeDraw,
									  const VertexArrayObject& vertexArray, const VertexBufferObject& buffObj, const bool IsAutoUnBind = true) = 0;
			virtual void CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeStorageDraw& typeDraw,
									  const VertexArrayObject& vertexArray, const bool IsAutoUnBind = true) = 0;
			virtual void CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeStorageDraw& typeDraw,
									  const bool IsAutoUnBind = true) = 0;
			virtual void DeleteBuffer() = 0;

			virtual bool SetData(const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind = true) = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;

			virtual uint32 GetBufferID() const = 0;
		};
	} // namespace Render
} // namespace CoreEngine
