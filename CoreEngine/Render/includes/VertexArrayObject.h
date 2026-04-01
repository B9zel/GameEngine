#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Core/includes/Base.h>

namespace CoreEngine
{
	namespace Render
	{
		class VertexBufferObject;
		class ElementBufferObject;
		class RenderDevice;

		class VertexArrayObject
		{
		protected:

			VertexArrayObject() = default;

		public:

			virtual ~VertexArrayObject() = default;

			static UniquePtr<VertexArrayObject> CreateVertexObject();

			virtual void CreateVertexArray() = 0;
			virtual void SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
										const VertexBufferObject& bufferObject, const uint32 beginStep = 0) = 0;

			virtual void SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
										const VertexBufferObject& bufferObject, const ElementBufferObject& elementObject, const uint32 beginStep = 0) = 0;
			virtual void SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
										const uint32 beginStep = 0) = 0;

			virtual void DeleteVertexObject(RenderDevice* Device) = 0;

			virtual void Bind(RenderDevice* Device) const = 0;
			virtual void UnBind() const = 0;

			virtual RHI::HandleVAO GetHandle() const = 0;
		};
	} // namespace Render
} // namespace CoreEngine
