#pragma once
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Core/includes/Platform.h>
#include <Core/includes/Base.h>

namespace CoreEngine
{
	namespace Render
	{
		class VertexArrayObject;

		class VertexBufferObject
		{
		public:

			VertexBufferObject(VertexBufferObject&&) = default;
			virtual ~VertexBufferObject() = default;

			static UniquePtr<VertexBufferObject> CreateVertexBufferObject();

			virtual void CreateBuffer(const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeStorageDraw& typeDraw,
									  const VertexArrayObject& vertexArray, const bool IsAutoUnBind = true) = 0;
			virtual void CreateBuffer(const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeStorageDraw& typeDraw,
									  const bool IsAutoUnBind = true) = 0;

			virtual void DeleteBuffer() = 0;

			virtual bool SetData(const int32 beginFillData, const uint32 sizeOfData, const void* data, const bool isBind = true) = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;

			virtual uint32 GetBufferID() const = 0;
		};
	} // namespace Render

} // namespace CoreEngine
