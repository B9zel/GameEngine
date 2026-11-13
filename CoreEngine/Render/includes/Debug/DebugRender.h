#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>

namespace CoreEngine::Render
{
	struct DebugLine
	{
		FVector Start;
		FVector End;
		FVector Color;
	};

	class DebugRender
	{
	public:

		virtual ~DebugRender() = default;

		void AddLine(const FVector& Start, const FVector& End, const FVector& Color);
		void ClearLines();

		virtual void Flush(const FMatrix4x4& view, const FMatrix4x4& projection) = 0;

	protected:

		DArray<DebugLine> DebugLines;
	};
} // namespace CoreEngine::Render
