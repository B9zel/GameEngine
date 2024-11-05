#pragma once
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/VertextBufferObject.h>
#include <Render/includes/ElementBufferObject.h>
#include <Core/includes/Base.h>


namespace CoreEngine
{
	namespace Render
	{
		class VertexArrayObject
		{
		public:

			virtual void SetupIntorprit(uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData, const VertexBufferObject& bufferObject, const uint32 beginStep=0) = 0;

			virtual void SetupIntorprit(uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData, const VertexBufferObject& bufferObject, const ElementBufferObject& elementObject, const uint32 beginStep=0) = 0;
			
			virtual void DeleteVertexObject() = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;
		};
	}
}