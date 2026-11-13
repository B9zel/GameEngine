#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <Math/includes/Vector.h>

namespace CoreEngine::Render
{
	class DebugRender;
}

namespace CoreEngine::Render::Debug
{
	class PhysDebugDraw final : public btIDebugDraw
	{
	public:

		explicit PhysDebugDraw(DebugRender& Render) : Renderer(Render)
		{
		}

	public:

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		virtual void setDebugMode(int Mode) override;
		virtual int getDebugMode() const override;

		virtual void reportErrorWarning(const char* Message) override;

		virtual void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override
		{
		}

		virtual void draw3dText(const btVector3&, const char*) override
		{
		}

	private:

		FVector ConvertToCustomVector(const btVector3& vec);

	private:

		DebugRender& Renderer;
		int32 m_Mode = DBG_DrawWireframe | DBG_DrawAabb;
	};
} // namespace CoreEngine::Render::Debug
