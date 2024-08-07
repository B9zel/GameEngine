#pragma once
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/VertextBufferObject.h>


namespace CoreEngine
{
	namespace Render
	{
		class VertexArrayObject
		{
		public:

			virtual void SetupIntorprit(unsigned int location, unsigned int sizeArgument, unsigned int step, const ETypeData& typeData, const VertexBufferObject& bufferObject, const unsigned int beginStep=0) = 0;
			
			virtual void DeleteVertexObject() = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;
		};
	}
}