#pragma once
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/Base.h>



namespace CoreEngine
{
	class UUID;

	class StaticMeshProxy : public PrimitiveProxy
	{
	public:

		StaticMeshProxy() = default;

		void AddIndeces(const DArray<uint32>& Indeces);
		const DArray<const DArray<uint32>*>& GetIndeces() const;
		
		void AddArrayObject(const Render::VertexArrayObject* VertexArray);
		const DArray<const Render::VertexArrayObject*>& GetArrayObject() const;


		void ClearData();

	private:

		DArray<const DArray<uint32>*> m_CountIndeces;
		DArray<const Render::VertexArrayObject*> m_ArrayObject;
	};
}