#include <Render/includes/Debug/PhysDebugDraw.h>
#include <LinearMath/btVector3.h>
#include <Render/includes/Debug/DebugRender.h>

namespace CoreEngine::Render::Debug
{
	void PhysDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Renderer.AddLine(ConvertToCustomVector(from), ConvertToCustomVector(to), ConvertToCustomVector(color));
	}

	void PhysDebugDraw::setDebugMode(int Mode)
	{
		m_Mode = Mode;
	}

	int PhysDebugDraw::getDebugMode() const
	{
		return m_Mode;
	}

	void PhysDebugDraw::reportErrorWarning(const char* Message)
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::WARNING, Message);
	}

	FVector PhysDebugDraw::ConvertToCustomVector(const btVector3& vec)
	{
		return FVector(vec.x(), vec.y(), vec.z());
	}
} // namespace CoreEngine::Render::Debug
