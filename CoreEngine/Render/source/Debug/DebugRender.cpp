#include <Render/includes/Debug/DebugRender.h>

namespace CoreEngine::Render
{
	void DebugRender::AddLine(const FVector& Start, const FVector& End, const FVector& Color)
	{
		DebugLine Line;
		Line.Start = Start;
		Line.End = End;
		Line.Color = Color;
		DebugLines.push_back(Line);
	}
	void DebugRender::ClearLines()
	{
		DebugLines.clear();
	}
} // namespace CoreEngine::Render
