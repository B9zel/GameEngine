#pragma once
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/Base.h>

namespace CoreEngine
{
	class UUID;
	namespace Render
	{
		class VertexArrayObject;
	}

	class StaticMeshProxy : public PrimitiveProxy
	{
	public:

		StaticMeshProxy() = default;

		void AddIndeces(const DArray<uint32>& Indeces);
		const DArray<const DArray<uint32>*>& GetIndeces() const;

		void AddArrayObject(const Render::RHI::HandleVAO& VertexArray);
		const DArray<Render::RHI::HandleVAO>& GetArrayObject() const;

		void ClearData();

	private:

		DArray<const DArray<uint32>*> m_CountIndeces;
		DArray<Render::RHI::HandleVAO> m_ArrayObject;
	};
} // namespace CoreEngine
