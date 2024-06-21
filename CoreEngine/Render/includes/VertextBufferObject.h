#pragma once
#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/Enums/TypeDraw.h>


namespace CoreEngine
{
	namespace Render
	{

		class VertexBufferObject
		{
		public:

			virtual void CreaterBuffer(const void* vertexArr, unsigned int sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) = 0;

			virtual void DeleteBuffer() = 0;

			virtual bool SetData(const void* vertexArr, unsigned int sizeArr, const ETypeData& typeArr, const ETypeDraw& typeDraw) = 0;

			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;
		};
	}

}