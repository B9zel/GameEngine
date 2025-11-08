#include <Core/includes/StaticMeshProxy.h>


namespace CoreEngine
{
	void StaticMeshProxy::AddIndeces(const DArray<uint32>& Indeces)
	{
		m_CountIndeces.push_back(&Indeces);
	}
	const DArray<const DArray<uint32>*>& StaticMeshProxy::GetIndeces() const
	{
		return m_CountIndeces;
	}
	void StaticMeshProxy::AddArrayObject(const Render::VertexArrayObject* VertexArray)
	{
		m_ArrayObject.push_back(VertexArray);
	}
	const DArray<const Render::VertexArrayObject*>& StaticMeshProxy::GetArrayObject() const
	{
		return m_ArrayObject;
	}
	void StaticMeshProxy::ClearData()
	{
		m_CountIndeces.clear();
		PositionLights.clear();
		m_ArrayObject.clear();

	}
	
}

