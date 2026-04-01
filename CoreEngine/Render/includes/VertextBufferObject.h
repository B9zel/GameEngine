#pragma once
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Core/includes/Platform.h>
#include <Core/includes/Base.h>

namespace CoreEngine
{
	namespace Render
	{
		class VertexArrayObject;
		class RenderDevice;

		class VertexBufferObject
		{
		public:

			VertexBufferObject() = default;
			VertexBufferObject(VertexBufferObject&&) = default;
			virtual ~VertexBufferObject() = default;

			static UniquePtr<VertexBufferObject> CreateVertexBufferObject();

			virtual void CreateBuffer(RenderDevice* Device, const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeStorageDraw& typeDraw,
									  const VertexArrayObject& vertexArray, const bool IsAutoUnBind = true) = 0;
			virtual void CreateBuffer(RenderDevice* Device, const void* vertexArr, uint32 sizeArr, const ETypeData& typeArr, const ETypeStorageDraw& typeDraw,
									  const bool IsAutoUnBind = true) = 0;

			virtual void DeleteBuffer() = 0;

			virtual bool SetData(RenderDevice* Device, const int32 beginFillData, const uint32 sizeOfData, const void* data, const bool isBind = true) = 0;

			virtual void Bind(RenderDevice* Device) const = 0;
			virtual void UnBind() const = 0;

			virtual RHI::BufferHandle GetHandle() const = 0;
		};
	} // namespace Render

} // namespace CoreEngine
